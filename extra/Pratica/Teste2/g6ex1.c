#include <detpic32.h>
void main(void)
{
	// Configure all (digital I/O, analog input, A/D module, interrupts)
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0; //RB14 analog input
	AD1CHSbits.CH0SA = 14;	
	AD1CON2bits.SMPI = 0; // 1 sample converted
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 16;
	AD1CON3bits.SAMC = 16;
	AD1CON1bits.ON = 1;

	IPC6bits.AD1IP = 1;
	IEC1bits.AD1IE = 1;
	IFS1bits.AD1IF = 0; // Reset AD1IF flag
	EnableInterrupts(); // Global Interrupt Enable
	// Start A/D conversion
	AD1CON1bits.ASAM = 1;
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
	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0; // Reset AD1IF flag
}

