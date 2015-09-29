#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <sched.h>
#include <sys/mman.h>

#include <native/task.h>
#include <native/timer.h> //for TM_NOW

#include "io.h"


#define CHAN1 1
#define CHAN2 2
#define CHAN3 3

#define LOW  0
#define HIGH 1

#define CPU_USED 1


void task1_function ( void );
void task2_function ( void );
void task3_function ( void );
void disturb ( void );
void checkChannel ( int channel );
int set_cpu(int cpu_number);
void timespec_add_us(struct timespec *t, long us);

int main(int argc, char **argv)
{
	mlockall(MCL_CURRENT|MCL_FUTURE); //lock current mem alloc and future mem alloc main mem

	io_init();
	rt_print_auto_init(1); //hvis vi trenger printf
	/*
	int rt_task_create(RT_TASK *task, const char *name, int stksize, int prio, int mode);
	int rt_task_start(RT_TASK *task, void(*entry)(void *cookie), void *cookie);
	int rt_task_set_periodic(RT_TASK *task, RTIME idate, RTIME period); */
	
	RT_TASK task1;
	RT_TASK task2;
	RT_TASK task3;	
	RTIME task_period = 1e6; //i nanosekunder 1e5 = 0.1ms

	rt_task_create(&task1, "task 1", 0, 99, 0);
	rt_task_create(&task2, "task 2", 0, 99, 0);
	rt_task_create(&task3, "task 3", 0, 99, 0);

	rt_task_start(&task1, &task1_function, NULL);
	rt_task_start(&task2, &task2_function, NULL);
	rt_task_start(&task3, &task3_function, NULL);

	rt_task_set_periodic(&task1, TM_NOW, task_period);
	rt_task_set_periodic(&task2, TM_NOW, task_period);
	rt_task_set_periodic(&task3, TM_NOW, task_period);	

	
	
	int i;
	pthread_t disturbPtrs[10];
	/*

	for ( i = 0 ; i < 10 ; i++ ) {
		pthread_create(&disturbPtrs[i], NULL, disturb,(void*) i );
	}

	for ( i = 0 ; i < 10 ; i++ ) {
		pthread_join(disturbPtrs[i],NULL);
	}
	*/
	while(1){}
	return 0;
}

void task1_function ( void ) {
	
	checkChannel(CHAN1);	
}

void task2_function ( void ) {	
	
	checkChannel(CHAN2);
}

void task3_function ( void ) {

	checkChannel(CHAN3);
}


void checkChannel ( int channel ) {

	set_cpu(CPU_USED);
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);


	while(1){

		timespec_add_us(&t,100);
		clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&t,NULL);
		
		if ( ! io_read(channel) ) {
			io_write(channel,LOW);
			usleep(5);
			io_write(channel,HIGH);
		}		
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

void timespec_add_us(struct timespec *t, long us){
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;
	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000){
		t->tv_nsec = t->tv_nsec - 1000000000;
		t->tv_sec += 1;
	}
} 


