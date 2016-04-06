#include <detpic32.h>
#define CALIBRATION_VALUE = 6000;

void delay(unsigned int n_intervals)
{	
	volatile unsigned int i;
	for( ; n_intervals != 0; n_intervals--)
		for(i = CALIBRATION_VALUE; i != 0; i--)
			;
}

void main(void)
{
	while(1)
	{
		resetCoreTimer();
		delay(1);
		printInt(readCoreTimer(),10 +(10 << 16));
		printStr("\r\n");
	}
}
