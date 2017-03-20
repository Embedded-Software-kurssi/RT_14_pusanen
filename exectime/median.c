#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
/* Size of the data buffer; length of the sequence. */
#define NWIDTH 11
/* Smaller than
 any datum */ 
#define STOPPER 0 


int medfilter(int datum);


int main(void)
{
    for (int i = 1; i < NWIDTH; i++) {
		medfilter(rand());
	}

	unsigned int median = medfilter(rand());
}



int medfilter(int datum)
{
	struct pair
	{
		/* Pointers forming list linked in sorted order */
		struct pair   *point;
		/* Values to sort */
		unsigned int  value;
	};
	/* Buffer of nwidth pairs */
	static struct pair buffer[NWIDTH];
	/* pointer into circular buffer of data */
	static struct pair *datpoint={buffer};
	/* chain stopper. */
	static struct pair small={NULL,STOPPER} ;
	/* pointer to head (largest) of linked
	list.*/
	static struct pair big={&small,0} ;
	/* pointer to successor of replaced data item */
	struct pair *successor   ;
	/* pointer used to scan down the sorted list */
	struct pair *scan        ;
	/* previous value of scan */
	struct pair *scanold     ;
	/* pointer to median */
	struct pair *median;     ;
	/* No stoppers allowed. */
	if(datum == STOPPER) datum = STOPPER + 1;
	/* increment and wrap data in pointer.*/
	
	if( (++datpoint - buffer) >= NWIDTH) datpoint=buffer;
	/* Copy in new datum */
	datpoint->value=datum        ;
	/* save pointer to old value's successor */
	successor=datpoint->point    ;
	/* median initially just before head of chain */
	median = &big                ;
	/* scanold initially points to scan pointer. */
	scanold = &big               ;
	/* points to first (largest) datum in chain */
	scan = big.point             ;
	
	/* loop
	through the chain */

	for( ; ; )
	{
		/* Handle odd-numbered item in chain  */
		/* Chain out the old datum.*/
		if( scan->point == datpoint ) scan->point = successor;
		/* If datum is larger than scanned value,*/
		if( (scan->value
		<
		datum) )
		{
			/* chain it in here.  */
			datpoint->point = scanold->point;
			/* mark it chained in. */
			scanold->point = datpoint;
			datum = 0;
			
		};
		/* Step median pointer down chain after doing odd-numbered element */
		/* Step median pointer.  */
		median = median->point       ;
		/* Break at end of chain  */
		if ( scan == &small ) break ;
		/* Save this pointer and   */
		scanold = scan ;
		/* step down chain */
		scan = scan->point ;
		/* Handle even-numbered item. As above without the median step.  */
		if(
		scan->point == datpoint ) scan->point = successor;
		if( (scan->value
		<
		datum) )
		{
			datpoint->point = scanold->point;
			scanold->point = datpoint;
			datum = 0;
		};
		if ( scan == &small ) break;
		scanold = scan ;
		scan = scan->point;
	};
	return( median->value );
}