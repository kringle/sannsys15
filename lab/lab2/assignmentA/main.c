#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


#define NUMBER_OF_TIMES 10

int globalVar = 10;


int main(int argc, char **argv)
{

	int localVar = 100;
	
	pid_t pid = fork();

	if ( pid == -1 ){
		perror("fork failed");
		exit(EXIT_FAILURE);
		
	}
	
	
	int i;

	for ( i = 0; i < NUMBER_OF_TIMES ; i++ ) {
		globalVar++;
		localVar++;
		printf("GlobalVar = %i, localVar = %i, pid= %i \n", globalVar,localVar,pid);		
	}

	
	return 0;
}

