#include<detpic32.h>
#define CALIBRATION_VALUE 6000

void delay(unsigned int n_intervals)
{
	volatile unsigned int i;
	
	for(; n_intervals != 0; n_intervals--)
		for(i = CALIBRATION_VALUE; i != 0; i--)
			;
}

void main(void){
	TRISB = TRISB & 0xFC00; //RB0-RB9 saídas
	static int codes[] = {0x77,0x41,0x3B,0x6B,0x4D,0x6E,0x7E,0x43,0x7F,0x6F,0x5F,0x7C,0x36,0x79,0x3E,0x1E};
	int i,j;
	while(1){
		//select display low
		LATBbits.LATB9 = 1;
		LATBbits.LATB8 = 0;
		LATB = LATB & 0xFf00;

		for(i = 0; i < 16; i++)
		{
			LATB = LATB | codes[i];
			delay(100);
		}
		LATB = LATB & 0xFf00;
		LATBbits.LATB8 = 1;
		LATBbits.LATB9 = 0;

		for(j = 0; j < 16; j++)
		{
			LATB = LATB & 0xFf00;
			LATB = LATB | codes[j];
			delay(1000);
		}
	}
}
