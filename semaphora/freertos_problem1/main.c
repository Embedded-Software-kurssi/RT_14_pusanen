/****************************************************************************
* FreeRTOS data race example / JPP 22032017
*
* 
****************************************************************************/
#include <avr/io.h> 
#include <util/delay.h>
#include <stdio.h>

/* FreeRTOS include files are located in FreeRTOS/sources/include, which must be set as an additional
   include path in project */
#include "FreeRTOS.h"
#include "task.h"

// in FreeRTOS, higher priority value means higher priority 
#define WRITER_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )
#define READER_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 )

static void vWriterTask( void *pvParameters );
static void vReaderTask( void *pvParameters );

int main( void )
{
    // create tasks and start scheduler
	xTaskCreate( vWriterTask, ( const char * ) "T1", 255, (void *)('1'), WRITER_TASK_PRIORITY, NULL );
	xTaskCreate( vReaderTask, ( const char * ) "T2", 255, (void *)('2'), READER_TASK_PRIORITY, NULL );
	vTaskStartScheduler();
	return 0;
}

volatile int shared;

static void vWriterTask( void *pvParameters )
{
	( void ) pvParameters;
	while(1)
	{
		shared=0x0000;
		shared=0xFFFF;
	}
}

static void vReaderTask( void *pvParameters )
{
	( void ) pvParameters;
	DDRD |= 0x80;
	while(1)
	{
		switch(shared)
		{
			//variable should be either 0x0000 or 0xFFFF
			case 0x0000:
			case 0xFFFF:
				//OK
				PORTD = 0x00;
				break;
			//but, catch the following failure with breakpoint:
			default:
				PORTD = 0x80;
				break;
		}
		vTaskDelay( 1 );
	}
}


