#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/times.h> 
#include <unistd.h>

void busy_wait_delay(int seconds);

static void printFunction( void *arg)
{
	printf("Starting voidprint\n");
	sleep(5);	
	//busy_wait_delay(5);
	printf("Stopping voidprint\n");
}

int main()
{

	printf("Starting programs\n");



	pthread_t thread1;
	pthread_t thread2;

	pthread_create(&thread1, NULL,printFunction,(void *) 1);
	pthread_create(&thread2, NULL,printFunction,(void *) 2);
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);



	return 0;
}


void busy_wait_delay(int seconds) 
{ 
     	int i, dummy; 
     	int tps = sysconf(_SC_CLK_TCK); 
 	clock_t start; 
     	struct tms exec_time; 
 	times(&exec_time); 
     	start = exec_time.tms_utime; 
     	while( (exec_time.tms_utime - start) < (seconds * tps)) { 
          	for(i=0; i<1000; i++){ 
               		dummy = i; 
  		} 
  		times(&exec_time); 
 	}	 	 
} 
