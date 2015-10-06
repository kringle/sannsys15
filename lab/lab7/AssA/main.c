#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <sched.h>
#include <sys/mman.h>
#include <native/sem.h>

#include <native/task.h>
#include <native/timer.h> //for TM_NOW

#include "io.h"


#define CHAN1 1
#define CHAN2 2
#define CHAN3 3

#define LOW  0
#define HIGH 1

#define CPU_USED 1
#define SEM_INIT_COUNT 0
#define SEM_MODE S_FIFO


// functions
void task ( void );

int set_cpu(int cpu_number);
void timespec_add_us(struct timespec *t, long us);

// semaphores
RT_SEM xenomai_semaphore;



int main(int argc, char **argv)
{

	
	mlockall(MCL_CURRENT|MCL_FUTURE); //lock current mem alloc and future mem alloc main mem

	io_init();
	rt_print_auto_init(1); //hvis vi trenger printf
	
	rt_sem_create(&xenomai_semaphore, "Vaarsemafor", SEM_INIT_COUNT, SEM_MODE);
	
	
	RT_TASK task1;
	RT_TASK task2;		
	//RTIME task_period = 1e3; //i nanosekunder 1e5 = 0.1ms

	#define AUT0_STCK_SIZE 0
	#define SINGLE_CORE_MODE T_CPU(0)
	rt_task_create(&task1, "task 1", AUT0_STCK_SIZE, 90, SINGLE_CORE_MODE);
	rt_task_create(&task2, "task 2", AUT0_STCK_SIZE, 80, SINGLE_CORE_MODE);
	rt_task_shadow (NULL, "main", 99,SINGLE_CORE_MODE);
	

	rt_task_start(&task1, &task, NULL);
	rt_task_start(&task2, &task, NULL);
	
	
	
	rt_printf("Started program\n");	
	rt_task_sleep(1000000000);		
	rt_sem_broadcast(&xenomai_semaphore);
		

	rt_sem_delete(&xenomai_semaphore);
	return 0;
}


void task ( void ) {

	usleep(100);
	rt_sem_p(&xenomai_semaphore, TM_INFINITE);
	usleep(100);
	rt_printf("task ran well\n");
	rt_sem_v(&xenomai_semaphore);
	
		
	
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



void timespec_add_us(struct timespec *t, long us){
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;
	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000){
		t->tv_nsec = t->tv_nsec - 1000000000;
		t->tv_sec += 1;
	}
} 


