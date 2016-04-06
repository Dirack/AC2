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
	static const unsigned char codes[] = {0x02,0x01,0x40,0x20,0x10,0x04,0x08};
	int i,j;
	while(1){
		//select display low
		LATBbits.LATB9 = 1;
		LATBbits.LATB8 = 0;
		LATB = LATB & 0xFf00;

		for(i = 0; i < 7; i++)
		{
			LATB = LATB | codes[i];
			delay(1000);
		}
		LATB = LATB & 0xFf00;
		LATBbits.LATB8 = 1;
		LATBbits.LATB9 = 0;

		for(j = 0; j < 7; j++)
		{
			LATB = LATB | codes[j];
			delay(1000);
		}
	}
}
