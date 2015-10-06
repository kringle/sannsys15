#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <sched.h>
#include <sys/mman.h>
#include <native/sem.h>
#include <native/mutex.h>

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
#define SEM_RES_INIT_COUNT 1
#define SEM_MODE S_FIFO
#define AUT0_STCK_SIZE 0
#define SINGLE_CORE_MODE T_CPU(0)
#define ONE_SEC 1e9	
#define TIME_UNIT 1e8 //100ms
#define TIME_UNIT_MS 100


#define TASKL_PRI	97
#define TASKH_PRI	98
#define MAX_PRI 	99


// functions
void taskL ( void );
void taskM ( void );
void taskH ( void );
void busy_wait_ms(unsigned long delay);
void print_pri(RT_TASK *task, char *s);


// semaphores
RT_SEM xenomai_semaphore;
RT_SEM resource_semaphore;

// mutex
RT_MUTEX mutexA;
RT_MUTEX mutexB;


// tasks
RT_TASK taskLow;
RT_TASK taskMed;
RT_TASK taskHigh;



int main(int argc, char **argv)
{	
	mlockall(MCL_CURRENT|MCL_FUTURE); //lock current mem alloc and future mem alloc main mem

	
	rt_print_auto_init(1); //hvis vi trenger printf
	
	rt_sem_create(&xenomai_semaphore, "Vaarsemafor", SEM_INIT_COUNT, SEM_MODE);
	rt_sem_create(&resource_semaphore, "ressursemafor", SEM_RES_INIT_COUNT, SEM_MODE);
	
	
	rt_mutex_create(&mutexA,"mutex A");
	rt_mutex_create(&mutexB,"mutex B");		

	
	rt_task_create(&taskLow, "task L", AUT0_STCK_SIZE, TASKL_PRI, SINGLE_CORE_MODE);	
	rt_task_create(&taskHigh, "task H", AUT0_STCK_SIZE, TASKH_PRI, SINGLE_CORE_MODE);
	rt_task_shadow (NULL, "main", 99,SINGLE_CORE_MODE);
	

	rt_task_start(&taskLow, &taskL, NULL);	
	rt_task_start(&taskHigh, &taskH, NULL);	
	
	
	rt_printf("Started program\n");
	
	rt_task_sleep(ONE_SEC);
	rt_printf("One second passed\n");		
	rt_sem_broadcast(&xenomai_semaphore);
		
	while(1){
		rt_task_sleep(100000);
	}
	
	rt_sem_delete(&xenomai_semaphore);
	rt_sem_delete(&resource_semaphore);

	rt_mutex_delete(&mutexA);
	rt_mutex_delete(&mutexB);
	
	return 0;
}


void taskL ( void ) {

	rt_sem_p(&xenomai_semaphore, TM_INFINITE); 	// wait for synch
	print_pri(&taskLow, "TaskL started\n");	
	rt_mutex_acquire(&mutexA,NULL);	// take mutex A
	rt_task_set_priority(&taskLow, MAX_PRI);

	print_pri(&taskLow, "TaskL took A\n");

	busy_wait_ms(3*TIME_UNIT_MS);
	rt_mutex_acquire(&mutexB,NULL);	// take mutex B
	print_pri(&taskLow, "TaskL took B\n");
	busy_wait_ms(3*TIME_UNIT_MS);
	rt_mutex_release(&mutexB); // release mutex B
	print_pri(&taskLow, "TaskL released B\n");
	rt_mutex_release(&mutexA); // release mutex A
	print_pri(&taskLow, "TaskL released A\n");
	rt_task_set_priority(&taskLow, TASKL_PRI);
	busy_wait_ms(1*TIME_UNIT_MS);
	print_pri(&taskLow, "TaskL completed work\n");
	//print_pri(&taskLow, "TaskL released resource\n");
	
}

void taskH ( void ) {

	rt_sem_p(&xenomai_semaphore, TM_INFINITE); 	// wait for synch
	print_pri(&taskHigh, "TaskH started\n");
	rt_task_sleep(1*TIME_UNIT);
	
	rt_mutex_acquire(&mutexB,NULL);			// take mutex B	
	rt_task_set_priority(&taskHigh, MAX_PRI);
	print_pri(&taskHigh, "TaskH took B\n");	
	busy_wait_ms(1*TIME_UNIT_MS);
	rt_mutex_acquire(&mutexA,NULL);			// take mutex A
	print_pri(&taskHigh, "TaskH took A\n");	
	busy_wait_ms(2*TIME_UNIT_MS);
	rt_mutex_release(&mutexA); 			// release mutex A
	print_pri(&taskHigh, "TaskH released A\n");	
	rt_mutex_release(&mutexB); // release mutex B
	rt_task_set_priority(&taskHigh, TASKH_PRI);
	print_pri(&taskHigh, "TaskH released B\n");
	busy_wait_ms(1*TIME_UNIT_MS);
	print_pri(&taskHigh, "TaskH completed work\n");	
	//print_pri(&taskHigh, "TaskH released resource\n");
}

void busy_wait_ms(unsigned long delay){
	unsigned long count = 0;
	while (count <= delay*10){
		rt_timer_spin(1000*100);
		count++;
	}
}

void print_pri(RT_TASK *task, char *s)
{ 
	struct rt_task_info temp;
	rt_task_inquire(task, &temp);
	rt_printf("b:%i c:%i ", temp.bprio, temp.cprio);
	rt_printf(s);
}



