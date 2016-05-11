#include <detpic32.h>
#include <ptrodrigues.h>

void init_adc_ints(void)
{
	IFS1bits.AD1IF = 0;
	IEC1bits.AD1IE = 1;
	IPC6bits.AD1IP = 1;
}

int main(void)
{
	init_adc14(1);
	init_adc_ints();
	EnableInterrupts();
	AD1CON1bits.ASAM = 1;
	while(true);
	return 0;
}

void _int_(27) isr_adc(void)
{
	volatile uint *ADCpointer;
	ADCpointer 	= &ADC1BUF0; 
	printInt(*ADCpointer,(3 << 16) + 16);
	putChar('\n');
	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
}
