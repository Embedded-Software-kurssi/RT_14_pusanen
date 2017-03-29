/****************************************************************************
* Simple FreeRTOS application example / JPP 13032017
*
* Implements two tasks each printing out a message.
* 
* Example code supports tracing as well:
* In FreeRTOS tracing means that macros "traceTASK_SWITCHED_OUT()" 
* and "traceTASK_SWITCHED_IN()" are executed at context switch. 
* These macros are defined in file "FreeRTOSConfig.h".
* So it is possible to set output pins according to tasks being executed,
* log pin changes in simulator log file or with oscillosope.
****************************************************************************/
#include <avr/io.h> 
#include <util/delay.h>
#include <stdio.h>

/* FreeRTOS include files are located in FreeRTOS/sources/include, which must be set as an additional
   include path in project */
#include "FreeRTOS.h"
#include "task.h"

// in FreeRTOS, higher priority value means higher priority 
#define mainTEST_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )


static void vTestTask1( void *pvParameters );
static void vTestTask2( void *pvParameters );
/* FreeRTOS has several hooks available. If enabled in FreeRTOSConfig.h, the OS will call hooks 
   at each corresponding event  */

void vContextSwitch();

const TickType_t xFrequency = 1;

int main( void )
{
	//PORTD |= 0x01;
    // create tasks and start scheduler
	xTaskCreate( vTestTask1, ( const char * ) "T1", 255, (void *)('1'), mainTEST_TASK_PRIORITY, NULL );
	xTaskCreate( vTestTask2, ( const char * ) "T2", 255, (void *)('2'), mainTEST_TASK_PRIORITY, NULL );
	vTaskStartScheduler();
	return 0;
}

static void vTestTask1( void *pvParameters )
{
	( void ) pvParameters;
	vTaskSetApplicationTaskTag( NULL, ( void * ) 0x01 );
	uint8_t pinmask = 0x01;
	TickType_t xLastWakeTime;
	DDRD |= pinmask;
    for( ;; )
	{
		xLastWakeTime = xTaskGetTickCount();
        _delay_ms( 1 );   //simulate task work done 
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}

static void vTestTask2( void *pvParameters )
{
	( void ) pvParameters;
	vTaskSetApplicationTaskTag( NULL, ( void * ) 0x02 );
	uint8_t pinmask = 0x02;
	TickType_t xLastWakeTime;
	DDRD |= pinmask;
	for( ;; )
	{
		xLastWakeTime = xTaskGetTickCount();
		_delay_ms( 1 );   //simulate task work done
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}

void vContextSwitch() {
	if (PORTD == 0x01) {
		PORTD &= ~0x01;
		PORTD |= 0x02;
	} else {
		PORTD &= ~0x02;
		PORTD |= 0x01;
	}
}






