#include "detpic32.h" 
void delay(unsigned int);
void main(void)
{
	while(1)
	{
		resetCoreTimer();
		delay(1);
		printInt(readCoreTimer(), 10);
		printStr("\r\n");
	}
}
void delay(unsigned int n_intervals)
{
	volatile unsigned int i;
	for(; n_intervals != 0; n_intervals--);
	for(i = 4997; i != 0; i--);
}

