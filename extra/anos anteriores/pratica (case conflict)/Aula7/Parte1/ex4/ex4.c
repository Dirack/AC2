#include<detpic32.h>
#define CALIBRATION_VALUE 5000
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

volatile unsigned char value2display = 0; // Global variable

void configureAll(void){
	// Configure all (digital I/O, analog input, A/D module, interrupts)
	TRISB = TRISB & 0xFC00;
	TRISBbits.TRISB14 = 1;	//RB14 digital output disconnect

	AD1PCFGbits.PCFG14 = 0;	//RB14 configured as analog input (AN4)
	AD1CHSbits.CH0SA = 14;	//Selects AN14 as input for the A/D converter
	AD1CON2bits.SMPI = 7; 	//8 sample converted and stored in ADC1BUF0
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON1bits.ON = 1; 	//Enable A/D

	IPC6bits.AD1IP = 1;
	IEC1bits.AD1IE = 1;
	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0; // Reset AD1IF flag
	
	T3CONbits.TCKPS = 2;
	PR3 = 49999;
	TMR3 = 0;
	T3CONbits.TON = 1;
	IPC3bits.T3IP = 1;
	IEC0bits.T3IE = 1;

	T1CONbits.TCKPS = 3;
	PR1 = 19531;
	TMR1 = 0;
	T1CONbits.TON = 1;
	IPC1bits.T1IP = 1;
	IEC0bits.T1IE = 1;
	EnableInterrupts(); // Global Interrupt Enable
}
void main(void)
{	
	configureAll(); 
	while(1);	
}
void _int_(27) isr_adc(void)
{
	// Calculate buffer average (8 samples)
	int j =0,aux = 0, val = 0;
	int *p = (int* ) (&ADC1BUF0);
	for(j = 0; j < 8; j++){
		//printInt(p[j*4],0x0004000A);
		//printStr(" ");
		aux = p[j*4];
		val += (aux*33+511)/1023;
	}
	//printStr("\n");
	val = val/8;
	//printInt(val,10);
	//printStr("\n");
	// Convert voltage amplitude to decimal. Assign it to "value2display"
	value2display = val; 
	IFS1bits.AD1IF = 0; // Reset AD1IF flag
}
void _int_(4) isr_T1(void)
{
	// Start A/D conversion
	AD1CON1bits.ASAM = 1;

	// Reset T1IF flag
	IFS0bits.T1IF = 0;
}
void _int_(12) isr_T3(void)
{
	// Send "value2display" global variable to displays
	send2displays(value2display);
	// Reset T3IF flag
	IFS0bits.T3IF = 0;
}
