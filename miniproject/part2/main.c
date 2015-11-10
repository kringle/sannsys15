#include <stdio.h>
#include "miniproject.h"
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>


#define PERIOD 	0.003 // [s]
#define RUNTIME 0.5   // [s]

void setU ( float u );
void sendMsg ( char msg[20], int len );

struct udp_conn conn;


void PIreg 	( void );
void UDPlis 	( void );
void UDPresp 	( void );



char PI_receive_string[20];
char globalMsgArray[20];

pthread_mutex_t UDP_mut;
pthread_mutex_t newPIval_mut;
pthread_mutex_t newSIG_mut;

bool newPIval 	= false;
bool newSIG 	= false;



int main(int *argc, char argv[]) {


	udp_init_client(&conn, 9999, "192.168.0.1");
	
	


	pthread_mutex_init(&UDP_mut,NULL);
	pthread_mutex_init(&newPIval_mut,NULL);
	pthread_mutex_init(&newSIG_mut,NULL);
	

	pthread_t PI_thread;
	pthread_t listen_thread;
	pthread_t respond_thread;

	pthread_create(&PI_thread, 	NULL,	PIreg,	NULL );
	pthread_create(&listen_thread, 	NULL,	UDPlis,	NULL );
	pthread_create(&respond_thread, NULL,	UDPresp, NULL );
	
	pthread_join(PI_thread,NULL);
	pthread_join(listen_thread,NULL);
	pthread_join(respond_thread,NULL);


	
	return 0;
}




void setU ( float u ){

	static char send2Buf[20] = "SET:";
	char floatArray[20];
	snprintf(floatArray,20,"%f",u);

	int i;
	for ( i = 4; i < 11 ; i++){
		send2Buf[i] = floatArray[i-4];
	} 
	pthread_mutex_lock(&UDP_mut);
	udp_send(&conn,send2Buf,sizeof(send2Buf));
	pthread_mutex_unlock(&UDP_mut);
	
}

void sendMsg ( char msg[30], int len ){

	pthread_mutex_lock(&UDP_mut);
	udp_send(&conn,msg,len);
	pthread_mutex_unlock(&UDP_mut);
}

void UDPlis ( void ){

	char receiveBuf[100];	

	while(1){
		if(udp_receive(&conn,receiveBuf,sizeof(receiveBuf))){
			
			if(!strncmp(receiveBuf,"SI",2)){
				
				pthread_mutex_lock(&newSIG_mut);
				newSIG = true;
				pthread_mutex_unlock(&newSIG_mut);
				

			} else {
				
				pthread_mutex_lock(&newPIval_mut);
				strcpy(globalMsgArray,receiveBuf);
				newPIval = true;
				pthread_mutex_unlock(&newPIval_mut);
							}
		}	
	}
}

void UDPresp 	( void ){
		
	while(1){
		// wait for signal from listener		
		while(!newSIG){}
		sendMsg("SIGNAL_ACK",11);
		pthread_mutex_lock(&newSIG_mut);
		newSIG = false;
		pthread_mutex_unlock(&newSIG_mut);
		
	}	
}

void PIreg 	( void ){

	
	double error, integral, u, y;
	const double reference = 1;
	const double Kp = 10;
	const double Ki = 800;
	const double period = PERIOD;		
	int numRep = RUNTIME/PERIOD;

	struct timespec waitTime;
	clock_gettime(CLOCK_REALTIME, &waitTime);
	
	sendMsg("START",6);
	
	int i;
	for ( i = 0 ; i < numRep ; i++ ){

		timespec_add_us(&waitTime,PERIOD*1000000);
		clock_nanosleep(&waitTime);

		// request new value
		sendMsg("GET",4);
	
		// wait for value from listener
		while(!newPIval){ 
			/* wait */ 
		}
		
		// reset flag
		pthread_mutex_lock(&newPIval_mut);
		newPIval = false;
		pthread_mutex_unlock(&newPIval_mut);
		
		// extract value
		char * numVal = globalMsgArray + 8;	
		y = atof(numVal);


		// run PI-regulator
		error = reference - y;
		integral = integral + (error * period);
		u = Kp * error + Ki * integral;
		
		setU(u);
		
	}
	sendMsg("STOP",5);
}






