#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


#define NUMBER_OF_RESOURCES 	3
#define NUMBER_OF_THREADS 	5

sem_t countingSem;

void accessResource( void* );

int main(int argc, char **argv)
{


	
	sem_init(&countingSem, 0, NUMBER_OF_RESOURCES);
	
	pthread_t threads[NUMBER_OF_THREADS];
	
	int i;
	for ( i = 0 ; i < NUMBER_OF_THREADS ; i++ ){
		pthread_create(&threads[i], NULL,accessResource,(void *) i);
	}

	for ( i = 0 ; i < NUMBER_OF_THREADS ; i++ ){
		pthread_join(threads[i],NULL);
	}

	


	return 0;
}

void accessResource( void* id ){
	
	// take semaphore
	sem_wait(&countingSem);

	int i;
	for ( i = 0 ; i < 4 ; i++ ) {
		printf("Thread number %i accessing resource \n",(int)id); 		
		sleep(1);
	}

	// relase semaphore
	sem_post(&countingSem);

}
