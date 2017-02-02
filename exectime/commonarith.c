/*
 * commonarith.c 
 *
 * Created: 2.2.2017 13.56.06
 * Author : Pusanen
 */ 

#include <avr/io.h>

int main(void)
{
    volatile int a = 5,b = 5;
	volatile float h = 5, j = 5;
	
	
	int c = a + b;
	int e = a * b;
	int d = a >= b;
	
	float y = h >= j;
	float L = h + j;
	float p = h * j;
}
