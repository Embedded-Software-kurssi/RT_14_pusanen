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

int main( void )
{
    // create tasks and start scheduler
	xTaskCreate( vTestTask1, ( const char * ) "T1", 255, (void *)('1'), mainTEST_TASK_PRIORITY, NULL );
	xTaskCreate( vTestTask2, ( const char * ) "T2", 255, (void *)('2'), mainTEST_TASK_PRIORITY, NULL );
	vTaskStartScheduler();
	return 0;
}


static void vTestTask1( void *pvParameters )
{
	( void ) pvParameters;
	uint8_t pinmask = 0x01; 
	DDRD |= pinmask;
    for( ;; )
	{
		PORTD |= pinmask;
        _delay_ms( 1 );   //simulate task work done
        PORTD &= ~pinmask; 
        vTaskDelay( 1 );
	}
}

static void vTestTask2( void *pvParameters )
{
	( void ) pvParameters;
	uint8_t pinmask = 0x02;
	DDRD |= pinmask;
	for( ;; )
	{
		PORTD |= pinmask;
		_delay_ms( 1 );   //simulate task work done
		PORTD &= ~pinmask;
		vTaskDelay( 1 );
	}
}


