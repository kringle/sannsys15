#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <time.h>
#include "io.h"


int main(int argc, char **argv)
{

	io_init();

	while(1){

		sleep(1);
		io_write(1,1);
		printf("Interim\n\r");
		sleep(1);
		io_write(1,0);

	}


	return 0;
}
