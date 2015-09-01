#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUMBER_OF_TIMES 10

int globalVar = 100;


void aThreadedFunction ( void ) {

	int localVar = 10;


	globalVar++;
	localVar++;
	printf("GlobalVar = %i, localVar = %i \n", globalVar,localVar);		
	
}

int main(int argc, char **argv)
{



	pthread_t thread1;
	pthread_t thread2;

	pthread_create(&thread1, NULL,aThreadedFunction,(void *) 1);
	pthread_create(&thread2, NULL,aThreadedFunction,(void *) 2);
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
		


	return 0;
}
