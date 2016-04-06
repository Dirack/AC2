#include <detpic32.h>
#define CALIBRATION_VALUE 3000
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


int main(void)
{
	TRISE = TRISE & 0xFF00;
	TRISB = TRISB & 0xFC00;
	// Configure all (digital I/O, analog input, A/D module)
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0;
	AD1CHSbits.CH0SA = 14;
	AD1CON2bits.SMPI = 3;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.ON = 1;
	LATE = LATE & 0xFF00;
	LATE = LATE | 0x0055;
	int i = 0;
	int average;
	while(1)
	{
		resetCoreTimer();
		unsigned int timer = readCoreTimer();
		while( timer < 20000){
			timer = readCoreTimer();
		}	
		// Wait 10 ms using the core timer
		if(i++ == 25) // 250 ms
		{
			// Convert analog input (4 samples)
			AD1CON1bits.ASAM = 1;
			while(IFS1bits.AD1IF == 0);
			// Calculate buffer average
			int *p = (int*) (&ADC1BUF0);
			average = 0;
			for(i = 0; i < 4; i++){
				average += p[i*4];
			//	printInt(p[i*4],0x0004000A);
			//	printStr(" ");
			}
			//printStr("\n");
			average = average / 4;
			// Calculate voltage amplitude
			// Convert voltage amplitude to decimal
			average = (average*33+511)/1023;
			//printInt(average,10);
			//printStr("\n");
			IFS1bits.AD1IF = 0;
			i = 0;
		}
		// Send voltage value to displays
		delay(10);
		send2displays(average);
	}
	return 0;
}
