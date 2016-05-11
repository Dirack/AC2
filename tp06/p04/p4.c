#include <detpic32.h>
#include <ptrodrigues.h>
#define FREQT1 4	// ADC refresh rate
#define FREQT3 100 	// SSD refresh rate
#define preT1 3
#define preT3 2
#define ADCSAMPLES 4

volatile uint ADCvalue=0;

void _init_(uint nSamples)
{	
	init_adc14(nSamples);
	TRISB &= 0xFC00;
	static uint preSclA[] = {1,2,64,256};
	static uint preSclB[] = {1,2,4,8,16,32,64,256};
	T1CONbits.TCKPS = preT1;
	PR1 = ((PBCLK/preSclA[preT1])/FREQT1)-1;
	TMR1 = 0;
	T1CONbits.TON = 1;
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 1;
	IPC1bits.T1IP = 1;

	T3CONbits.TCKPS = preT3;
	PR3 = ((PBCLK/preSclB[preT3])/FREQT3)-1;
	TMR3 = 0;
	T3CONbits.TON = 1;
	IFS0bits.T3IF = 0;
	IEC0bits.T3IE = 1;
	IPC3bits.T3IP = 2;
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

void _int_(4) isr_adc(void)
{
	AD1CON1bits.ASAM = 1;
	uint mean = 0;
	uint *ADCPOINTER  = (uint*)&ADC1BUF0;
	int j;
	for(j=0; j < ADCSAMPLES; j++, ADCPOINTER = ADCPOINTER + 4)
	{
		mean += *ADCPOINTER;
	}
	 ADCvalue = ((mean / ADCSAMPLES)*33)/1023;
	 IFS1bits.AD1IF = 0;
	 IFS0bits.T1IF = 0;
}

void _int_(12) isr_SSD(void)
{
	send2SSD(toBCD(ADCvalue));
	IFS0bits.T3IF = 0;
}



int main(void)
{
	_init_(ADCSAMPLES);
	EnableInterrupts();
	while(true);
	return 0;
}
