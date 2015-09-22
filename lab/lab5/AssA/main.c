#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#include "io.h"


#define CHAN1 1
#define CHAN2 2
#define CHAN3 3

#define LOW  0
#define HIGH 1


void thread1 ( void );
void thread2 ( void ); 
void thread3 ( void );
void checkChannel ( int channel );

int main(int argc, char **argv)
{

	io_init();
	
	pthread_t thread_1;
	pthread_t thread_2;
	pthread_t thread_3;
	

	pthread_create(&thread_1, NULL,thread1,(void *) 1);
	pthread_create(&thread_2, NULL,thread2,(void *) 2);
	pthread_create(&thread_3, NULL,thread3,(void *) 3);
	


	pthread_join(thread_1,NULL);
	pthread_join(thread_2,NULL);
	pthread_join(thread_3,NULL);

	return 0;
}

void thread1 ( void ) {

	checkChannel(CHAN1);
}

void thread2 ( void ) {
	
	checkChannel(CHAN2);
}

void thread3 ( void ) {

	checkChannel(CHAN3);
}


void checkChannel ( int channel ) {

	while(1){
		while(io_read(channel))  // wait
		{}
	
		io_write(channel,LOW);
		usleep(5);
		io_write(channel,HIGH);
	}
}


