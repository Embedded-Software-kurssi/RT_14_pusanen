/****************************************************************************
* FreeRTOS priority inversion example / JPP 13042017
*
* Implements three tasks + debug task.
* T1 lowest priority (task release time 1, capacity 3, uses shared resource 3)
* T2 medium priority (task release time 2, capacity 5)
* T3 highest priority (task release time 3, capacity 3, uses shared resource 3)
*
* Note 1: Task release time = "the earliest time when a job/task can start execution"
* Note 2: Release times here are calculated from scheduler start. This setup takes
*         about 1.5 msec to run initializations from reset to scheduler start. 
*
* The debug task has very highest priority, so it can be used to create breakpoint
* at any time point. In this the breakpoint is set to time point 10, where T3 should
* be ready if priority inversion does not occur.
*
* This setup demonstrates priority inversion at time point 3, as T3 tries to take 
* semaphore. Taking the semaphore is blocked, because T1 has reserved the resource.
* This blocking is unavoidable. However, the blocking is further elongated by T2,
* which blocks T1 because it has higher priority than T1. So, effectively T3 is 
* waiting for lower priority task T2 to finish, even though they do share any 
* resources.
****************************************************************************/
#include <avr/io.h> 
#include <util/delay.h>
#include <stdio.h>

/* FreeRTOS include files are located in FreeRTOS/sources/include, which must be set as an additional
   include path in project */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// in FreeRTOS, higher priority value means higher priority 
#define DEBUG_TASK_PRIORITY			( tskIDLE_PRIORITY + 4 )
#define TASK1_PRIORITY		    	( tskIDLE_PRIORITY + 1 )
#define TASK2_PRIORITY		    	( tskIDLE_PRIORITY + 2 )
#define TASK3_PRIORITY		    	( tskIDLE_PRIORITY + 3 )

static void vDebugTask( void *pvParameters );
static void vTask1( void *pvParameters );
static void vTask2( void *pvParameters );
static void vTask3( void *pvParameters );
SemaphoreHandle_t xSharedResourceSem;
volatile int8_t ucTask3Ready = 0;

int main( void )
{
    // create tasks 
	xTaskCreate( vDebugTask, ( const char * ) "DBG", 150, (void *)0, DEBUG_TASK_PRIORITY, NULL );
	xTaskCreate( vTask1, ( const char * ) "T1", 150, (void *)1, TASK1_PRIORITY, NULL );
	xTaskCreate( vTask2, ( const char * ) "T2", 150, (void *)2, TASK2_PRIORITY, NULL );
	xTaskCreate( vTask3, ( const char * ) "T3", 150, (void *)3, TASK3_PRIORITY, NULL );
	// Create semaphore for sharing a resource between tasks 1 and 3 (shared resource is not implemented)
	// and initialize it to value one
	xSharedResourceSem = xSemaphoreCreateMutex();
	xSemaphoreGive(xSharedResourceSem);
	vTaskStartScheduler();
	return 0;
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
	while(1);
}

static void vDebugTask( void *pvParameters )
{
	( void ) pvParameters;
	// task release time = 10
	vTaskDelay( 10 );
	// just to check if task3 is ready, in any case we need to release cpu to lowefr priorities to see how
	// each case continues
	if (ucTask3Ready){
		//release CPU to lower priorities.
		while(1){
			vTaskDelay( 10 );
		}
	}
	while(1){
		vTaskDelay( 10 );
	}
}

// T1 has lowest priority, release time = 1 and it uses shared resource for 3 ticks 
static void vTask1( void *pvParameters )
{
	( void ) pvParameters;
	vTaskDelay( 1 );
	xSemaphoreTake(xSharedResourceSem, portMAX_DELAY);
	_delay_ms( 3 );  //simulate work with shared resource
	xSemaphoreGive(xSharedResourceSem);
	//done, release CPU to lower priorities.
	while(1)
	{
		vTaskDelay( 100 );	
	}
}

// T2 has medium priority, release time = 2, capacity = 5 and it does not use shared resource
static void vTask2( void *pvParameters )
{
	( void ) pvParameters;
	vTaskDelay( 2 );
	_delay_ms( 5 );  //simulate work 
	//done, release CPU to lower priorities.
	while(1)
	{
		vTaskDelay( 100 );	
	}
}

// T3 has high priority, release time = 3 and it uses shared resource for 3 ticks 
static void vTask3( void *pvParameters )
{
	( void ) pvParameters;
	vTaskDelay( 3 );
	xSemaphoreTake(xSharedResourceSem, portMAX_DELAY);
	_delay_ms( 3 );  //simulate work with shared resource
	xSemaphoreGive(xSharedResourceSem);
	ucTask3Ready = 1;
	//done, release CPU to lower priorities.
	while(1)
	{
		vTaskDelay( 100 );	
	}
}

