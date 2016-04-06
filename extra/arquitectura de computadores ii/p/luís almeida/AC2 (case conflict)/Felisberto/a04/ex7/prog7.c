#include "detpic32.h";
void delay(unsigned int);
void main(void)
{
	static const unsigned char 7Scodes[] = {0x77,0x41,0x3b,0x6b,0x4d,0x6e,0x7e,0x43,0x7f,0x4f}; //{0,1,2,3,4,5,6,7,8,9}
	TRISB = TRISB & 0xFC00;  	//1111 1100 0000 0000
	TRISE = TRISE | 0x00F0; 	//1111 1111 0000 0000
	int i;
	while(1)
	{	
		LATBbits.LATB8 = 0;
		LATBbits.LATB9 = 1;
		i = 0;
		if(PORTEbits.RE4==1)
		{
			i = i + 1;
		}
		if(PORTEbits.RE5==1)
		{
			i = i + 2;
		}
		if(PORTEbits.RE6==1)
		{
			i = i + 4;
		}
		if(PORTEbits.RE7==1)
		{
			i = i + 8;
		}
		if(i<=9)
		{
			LATB = (LATB & 0xFF00) | codes[i];
		}
		else
		{
			LATB = LATB & 0xFF00;
		}
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
