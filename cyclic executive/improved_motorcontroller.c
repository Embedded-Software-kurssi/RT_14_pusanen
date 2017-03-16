#include <Arduino.h>

/*
 Name:		taistelulaiva.ino
 Author:	pusanen
*/


const unsigned long	FRAME_MS = 10;
const unsigned long FRAMES = 10;
volatile int nextFrame = 0;

void task1();
void task2();
void task3();


void setup() {
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 2000;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set cs21 bit for 8 bit prescaler
  TCCR1B |= (1 << CS21); 
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}

void loop() {
	while(1) {
		static unsigned long frame = 0;
		while(!nextFrame);
		nextFrame = false;
		switch(frame) {
			case 0:
			task1();
			task2();
			break;
			case 1:
			task1();
			task3();
			break;
			case 2:
			task1();
			break;
			case 3:
			task1();
			break;
			case 4:
			task1();
			break;
			case 5:
			task1();
			break;
			case 6:
			task1();
			task3();
			break;
			case 7:
			task1();
			break;
			case 8:
			task1();
			break;
			case 9:
			task1();
			break;
		}
		frame = (frame + 1) % FRAMES;
	}
}

void task1() {
	for(volatile int i = 0; i < 252; i++) {
		
	}
}

void task2() {
	for(volatile int i = 0; i < 420; i++) {
		
	}
}

void task3() {
	for(volatile int i = 0; i < 588; i++) {
		
	}
}


ISR(TIMER1_COMPA_vect){
	nextFrame = 1;
}