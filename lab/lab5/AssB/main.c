#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <sched.h>

#include "io.h"


#define CHAN1 1
#define CHAN2 2
#define CHAN3 3

#define LOW  0
#define HIGH 1

#define CPU_USED 1


void thread1 ( void );
void thread2 ( void ); 
void thread3 ( void );
void disturb ( void );
void checkChannel ( int channel );
int set_cpu(int cpu_number);

int main(int argc, char **argv)
{

	io_init();
	
	pthread_t thread_1;
	pthread_t thread_2;
	pthread_t thread_3;
	

	pthread_create(&thread_1, NULL,thread1,(void *) 1);
	pthread_create(&thread_2, NULL,thread2,(void *) 2);
	pthread_create(&thread_3, NULL,thread3,(void *) 3);
	
	int i;
	pthread_t disturbPtrs[10];

	for ( i = 0 ; i < 10 ; i++ ) {
		pthread_create(&disturbPtrs[i], NULL, disturb,(void*) i );
	}

	for ( i = 0 ; i < 10 ; i++ ) {
		pthread_join(disturbPtrs[i],NULL);
	}

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

	set_cpu(CPU_USED);

	while(1){
		while(io_read(channel))  // wait
		{}
	
		io_write(channel,LOW);
		usleep(5);
		io_write(channel,HIGH);
	}
}

int set_cpu(int cpu_number){
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpu);
} 

void disturb( void ){

	set_cpu(CPU_USED);
	int someVar = 0;
	while(1){
		someVar++;
	}
}

