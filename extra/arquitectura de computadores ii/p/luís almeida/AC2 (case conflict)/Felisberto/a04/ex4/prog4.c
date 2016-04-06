#include "detpic32.h"
void delay(unsigned int);
void main(void)
{
	static const unsigned char codes[] = {0x02,0x01,0x40,0x20,0x10,0x04,0x08}; //{a,b,c,d,e,f,g}
	TRISB = TRISB & 0xFC00;
	while(1)
	{
		LATBbits.LATB8 = 0;
		LATBbits.LATB9 = 1;
		int i = 0;
		for(i=0;i<7;i++)
		{
			LATB = (LATB & 0xFF00) | codes[i];
			delay(1000);
		}
		LATBbits.LATB8 = 1;
		LATBbits.LATB9 = 0;
		for(i=0;i<7;i++)
		{
			LATB = (LATB & 0xFF00) | codes[i];
			delay(1000);
		}
		LATB = LATB & 0xFF00;
	}		
}
void delay(unsigned int n_intervals)
{
	volatile unsigned int i;
	for(; n_intervals != 0; n_intervals--)
{
	for(i = 4997; i != 0; i--);
}
}
