#include <stdio.h>
#include "miniproject.h"
#include <time.h>
#include <stdlib.h>


#define PERIOD 	0.005 // [s]
#define RUNTIME 0.5   // [s]

void setU ( float u );

struct udp_conn conn;

int main(int *argc, char argv[]) {


	char sendBuf[100];
	char receiveBuf[100];	
	struct timespec waitTime;
	clock_gettime(CLOCK_REALTIME, &waitTime);
	udp_init_client(&conn, 9999, "192.168.0.1");

	//VARIABLES FOR REGULATING
	double error, integral, u, y;
	double reference = 1;
	double Kp = 10;
	double Ki = 800;
	double period = PERIOD;

	strncpy(sendBuf,"START", sizeof(sendBuf));	
	udp_send(&conn,sendBuf,sizeof(sendBuf)); //strlen(sendBuf)+1
	
	//strncpy(sendBuf,"SET:12.3456",sizeof(sendBuf));	
	//udp_send(&conn,sendBuf,sizeof(sendBuf));

	
	int numRep = RUNTIME/PERIOD;

	int i;
	for ( i = 0 ; i < numRep ; i++ ){
		timespec_add_us(&waitTime,PERIOD*1000000);
		clock_nanosleep(&waitTime);

		strncpy(sendBuf,"GET", sizeof(sendBuf));
		udp_send(&conn,sendBuf,sizeof(sendBuf));
		udp_receive(&conn,receiveBuf,sizeof(receiveBuf));
	
		
		char * numVal = receiveBuf + 8;	
		y = atof(numVal);

		//printf("Y-value from server is %f\n",y);

			

		//WE HAVE A Y-VALUE, DO PI REGULATING
		error = reference - y;
		integral = integral + (error * period);
		u = Kp * error + Ki * integral;

		setU(u);
	
	
	}
	

	strncpy(sendBuf,"STOP", sizeof(sendBuf));
	udp_send(&conn,sendBuf,sizeof(sendBuf));
	
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

	udp_send(&conn,send2Buf,sizeof(send2Buf));

	//printf("%s\n", send2Buf); 

}


