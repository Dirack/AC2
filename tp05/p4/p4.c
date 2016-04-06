#include <detpic32.h>
#include <ptrodrigues.h>
#define ADC_samples 8
void init_adc_ints(void)
{
	IPC6bits.AD1IP = 1;
	IEC1bits.AD1IE = 1;
	IFS1bits.AD1IF = 0;
}

uchar toBCD(uchar character)
{
	return (((character/10)<<4) + (character % 10));
}

void send2SSD(uchar character)
{
	static uchar char_array[] = {
	ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,
	A,B,C,D,E,F,DP,0x00};
	static boolean high_display = false;

	if(high_display^1)
		character = char_array[character & 0xF];
	else
		character = char_array[character >> 4] | dp;

	PORTB = set_SSD_Hex(PORTB, character, high_display);
	high_display ^= 1;
}

volatile uint ADC_value = 0;

int main(void)
{
	init_adc14(ADC_samples);
	init_adc_ints();
	TRISB &= 0xFC00; 
	EnableInterrupts();
	while(true)
	{
		int i = 0;
		
		do
		{
			if((i++ % 25) == 0)
				AD1CON1bits.ASAM = 1;
			send2SSD(toBCD(ADC_value));
			printInt10(ADC_value / 10);
			putChar('.');
			printInt10(ADC_value % 10);
			putChar('\n');
		}while(i<1000);
	}
	return 0;
}
/*	OLD POINTER VERSION
void _int_(27) isr_adc(void)
{
	int i;
	int *p = (int *) (&ADC1BUF0);
	int V_mean = 0;
	for(i = 0; i < ADC_samples; i++)
	{
		V_mean += p[i*4];
	}
	ADC_value = ((V_mean/ADC_samples)*33)/1023;
	IFS1bits.AD1IF = 0;
}
*/

// NEW POINTER VERSION
void _int_(27) isr_adc(void)
{
	int i;
	volatile uint *ADCPointer = (uint *)&ADC1BUF0;
	uint V_mean = 0;
	for(i = 0; i < ADC_samples; i++)
	{
		V_mean = V_mean + *ADCPointer;
		ADCPointer++;
		ADCPointer += 4;
	}
	ADC_value = ((V_mean/ADC_samples)*33)/1023;
	IFS1bits.AD1IF = 0;
}
