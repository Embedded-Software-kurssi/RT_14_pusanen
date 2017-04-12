/****************************************************************************
* FreeRTOS data race example / JPP 22032017
*
* 
****************************************************************************/
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

/* FreeRTOS include files are located in FreeRTOS/sources/include, which must be set as an additional
   include path in project */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// in FreeRTOS, higher priority value means higher priority 
#define WRITER_TASK_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define READER_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )

static void vWriterTask( void *pvParameters );
static void vReaderTask( void *pvParameters );

SemaphoreHandle_t xSemaphore;

int main( void )
{
    // create tasks and start scheduler
	xTaskCreate( vWriterTask, ( const char * ) "T1", 255, (void *)('1'), WRITER_TASK_PRIORITY, NULL );
	xTaskCreate( vReaderTask, ( const char * ) "T2", 255, (void *)('2'), READER_TASK_PRIORITY, NULL );
	xSemaphore = xSemaphoreCreateBinary();
	xSemaphoreGive(xSemaphore);
	vTaskStartScheduler();
	return 0;
}

volatile int shared;

static void vWriterTask( void *pvParameters )
{
	( void ) pvParameters;

	while(1)
	{
		if (xSemaphore != NULL) {
			if (xSemaphoreTake(xSemaphore, (TickType_t) 10 ) == pdTRUE) 
			{
				shared=0x0000;
				shared=0xFFFF;
				xSemaphoreGive(xSemaphore);		
			}
		}
		vTaskDelay( 1 );
	}
}

static void vReaderTask( void *pvParameters )
{
	( void ) pvParameters;
	DDRD |= 0x80;
	while(1)
	{
		if (xSemaphore != NULL) {
			if (xSemaphoreTake(xSemaphore, (TickType_t) 10 ) == pdTRUE)
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
				xSemaphoreGive(xSemaphore);
			}
		}
	}
}


