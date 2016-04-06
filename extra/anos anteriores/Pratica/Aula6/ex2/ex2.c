#include <detpic32.h>

/*RE0 = 1 durante 2.6us */

void main(void)
{
	// Configure all (digital I/O, analog input, A/D module, interrupts)
	TRISEbits.TRISE0 = 0;
	TRISBbits.TRISB14 = 1;	//RB14 digital output disconnect

	AD1PCFGbits.PCFG14 = 0;	//RB14 configured as analog input (AN4)
	AD1CHSbits.CH0SA = 14;	//Selects AN14 as input for the A/D converter
	AD1CON2bits.SMPI = 0; 	//1 sample converted and stored in ADC1BUF0
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON1bits.ON = 1; 	//Enable A/D

	IPC6bits.AD1IP = 1;
	IEC1bits.AD1IE = 1;
	IFS1bits.AD1IF = 0; // Reset AD1IF flag
	EnableInterrupts(); // Global Interrupt Enable
	// Start A/D conversion
	AD1CON1bits.ASAM = 1;	//Start conversion
	while(1)
		; // do nothing (all activity is done by the ISR)
}
// Interrupt Handler
void _int_(27) isr_adc(void) // Replace VECTOR by the A/D vector
	// number - see "PIC32 family data
	// sheet" (pages 122-124)
{
	
	// Print ADC1BUF0 value // Hexadecimal (3 digits format)
	printInt(ADC1BUF0,0x00030010);
	printStr("\n");
	// Start A/D conversion
	AD1CON1bits.ASAM = 1;	//Start conversion
	
	IFS1bits.AD1IF = 0; // Reset AD1IF flag
}
