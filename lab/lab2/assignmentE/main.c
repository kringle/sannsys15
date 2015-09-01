#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_FORKS 	5
#define NUM_PHIL 	5

#define LEFT 		1
#define RIGHT 		2

#define GOT_FORK	3
#define FORK_BUSY 	4

pthread_mutex_t forks[NUM_FORKS];
pthread_t 	phils[NUM_PHIL];

int pickUpFork 			( int phil, int dir);
int putDownFork 		( int phil, int dir);
void phil			( void* id );
int findLowestIndexFork 	( int phil );
int findHighestIndexFork 	( int phil );
void  pickUpLowestFork 		( int phil);
void pickUpHighestFork 		( int phil);



int main(int argc, char **argv)
{
	//initialize mutex
	int i;
	for( i = 0 ; i < NUM_FORKS ; i++ ) {
		if ( pthread_mutex_init(&forks[i], NULL) != 0 ){
			printf("\n mutex init failed! \n ");
			exit(1);
		}
	}
	
	

	for ( i = 0 ; i < NUM_PHIL ; i++ ) {
		pthread_create(&phils[i], NULL,phil,(void *) i);
	}
	
	for ( i = 0 ; i < NUM_PHIL ; i++ ) {
		pthread_join(phils[i],NULL);
	}	

	return 0;
}

void phil ( void* id ){

	int philID = (int)id;

	while ( 1 ) {

		// think
	

		pickUpLowestFork(philID);
		pickUpHighestFork(philID);

		//pickUpFork(philID,LEFT);
		//pickUpFork(philID,RIGHT);
		// eat	
		usleep(100000);
		printf("Phil %i is eating \n", philID);
		putDownFork(philID, RIGHT);
		putDownFork(philID,LEFT);
		
		usleep(100000);
		
	 
	}


}

int findLowestIndexFork ( int phil ) {

	if ( phil == NUM_PHIL - 1 ) {
		return 0;
	} else {
		return phil; 
	}
	
}

int findHighestIndexFork ( int phil ) {
	
	if (  phil == NUM_PHIL - 1 ){
		return NUM_PHIL - 1;
	} else {
		return phil + 1; 
	}
}

void pickUpLowestFork ( int phil){


	int forkToPick = findLowestIndexFork ( phil );
	pthread_mutex_lock(&forks[forkToPick]);
	printf("Phil %i picks \n", phil);


}


void pickUpHighestFork ( int phil) {

	int forkToPick = findHighestIndexFork ( phil);
	pthread_mutex_lock(&forks[forkToPick]);
	printf("Phil %i picks \n", forkToPick);

}


int pickUpFork ( int phil, int dir){

	int mutexToLock;

	if ( dir == LEFT ){
		mutexToLock = phil;
	
	} else if ( dir == RIGHT ) {
		mutexToLock = phil+1;
		if ( phil == NUM_PHIL-1 ){
			mutexToLock = 0;		
		}
	}

	pthread_mutex_lock(&forks[mutexToLock]);
	printf("Phil %i picks \n", phil);

}

int putDownFork ( int phil, int dir){

	int mutexToUnlock;

	if ( dir == LEFT ){
		mutexToUnlock = phil;
	
	} else if ( dir == RIGHT ) {
		mutexToUnlock = phil+1;
		if ( phil == NUM_PHIL-1 ){
			mutexToUnlock = 0;		
		}
	}

	pthread_mutex_unlock(&forks[mutexToUnlock]);
	printf("Phil %i puts down \n", phil);
}
