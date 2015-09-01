#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>


pthread_mutex_t myMutex;

bool running 	= true;
int  var1 	= 0 ;
int  var2 	= 0;


void thread1 ( void );
void thread2 ( void ); 

int main(int argc, char **argv)
{

	if ( pthread_mutex_init(&myMutex, NULL) != 0 ){
		printf("\n mutex init failed! \n ");
		exit(1);
	}

	
	pthread_t thread_1;
	pthread_t thread_2;

	pthread_create(&thread_1, NULL,thread1,(void *) 1);
	pthread_create(&thread_2, NULL,thread2,(void *) 2);
	pthread_join(thread_1,NULL);
	pthread_join(thread_2,NULL);


	return 0;
}

void thread1 ( void ) {

	while ( running ) {
		//take mutex
		pthread_mutex_lock(&myMutex);
		var1++;
		var2 = var1;
		pthread_mutex_unlock(&myMutex);
		// release mutex
	}

}


void thread2 ( void ) {
	
	int i = 0;
	for ( i = 0 ; i < 20 ; i++ ) {

		pthread_mutex_lock(&myMutex);
		printf("Number 1 is: %i, Number 2 is: %i.\n",var1,var2);		
		pthread_mutex_unlock(&myMutex);
		
		usleep(100000);
	}
	
	running = false;

}
