#include<detpic32.h>
#define CALIBRATION_VALUE 6000

void delay(unsigned int n_intervals)
{
	volatile unsigned int i;
	
	for(; n_intervals != 0; n_intervals--)
		for(i = CALIBRATION_VALUE; i != 0; i--)
			;
}
void send2displays(unsigned char value)
{	
	unsigned char digit_low,digit_high;
	static unsigned char displayFlag = 0;
	static const unsigned char display7Scodes[] = {0x77,0x41,0x3B,0x6B,0x4D,0x6E,0x7E,0x43,0x7F,0x6F,0x5F,0x7C,0x36,0x79,0x3E,0x1E};
	
	digit_low = value & 0x0F;
	digit_high = value>>4;

	//if "displayFlag" is 0 send digit_low to display_low
	if(displayFlag == 0){
		LATBbits.LATB9 = 1;
		LATBbits.LATB8 = 0;
		LATB = LATB & 0xFF00;
		LATB = LATB | display7Scodes[digit_low];
	}
	else{
		LATBbits.LATB9 = 0;
		LATBbits.LATB8 = 1;
		LATB = LATB & 0xFF00;
		LATB = LATB | display7Scodes[digit_high];
	}
	displayFlag = !displayFlag;
}

void main(void){
	TRISB = TRISB & 0xFC00; //RB0-RB9 saídas
	unsigned char counter = 0;
	int i;
	while(1){
		i = 0;
		do{
			delay(10);
			send2displays(counter);
		}while(++i < 20);
			counter++;	
	}
}
