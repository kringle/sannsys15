#include <stdio.h>
#include "miniproject.h"
#include <time.h>

int main(int *argc, char argv[]) {


	struct udp_conn testMsg;
	char testBuf[100] = "Hello, AVR!";
	struct timespec waitTime;
	clock_gettime(CLOCK_REALTIME, &waitTime);

	


	
	udp_init_client(&testMsg, 9999, "192.168.0.1");
	while(1){

		udp_send(&testMsg,testBuf,strlen(testBuf)+1);
		printf("Hello world\n");
		timespec_add_us(&waitTime,500000);
		clock_nanosleep(&waitTime);		
	
	}
	return 0;



}
