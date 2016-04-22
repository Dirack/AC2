#include <detpic32.h>
#include <ptrodrigues.h>
#define FREQT1 50	// ADC refresh rate
#define FREQT3 100 	// SSD refresh rate
#define preT1 3
#define preT3 2
#define dc 25
#define ADCSAMPLES 4

volatile uint ADCvalue = 0;
volatile boolean SSD_status = true;

void _init_(uint nSamples)
{	
	init_adc14(nSamples);
	TRISB &= 0xFC00;
	TRISE &= 0xFFFF;
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

	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	OC1RS = 0;
	OC1CONbits.ON = 1;
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
	if(SSD_status)
		send2SSD(toBCD(ADCvalue));
	else
		LATB = LATB & 0xFCFF;
	IFS0bits.T3IF = 0;
}

void setPWM(uint DutyCycle)
{
	if(!(DutyCycle < 0 || DutyCycle > 100))
	{
		OC1RS = ((PR3+1) * DutyCycle) / 100;
	}
}

int main(void)
{
	_init_(ADCSAMPLES);
	EnableInterrupts();
	uchar dip_status;
	static uchar led_brightness[] = {3, 15, 40, 90};
	while(true)
	{
		dip_status = ((PORTE >> 4) & 0x3);
		if((dip_status >> 1)^0x1)
		{
			SSD_status = true;
			if(dip_status)
			{
				IEC0bits.T1IE = 0;
				setPWM(100);
			}
			else
			{
				IEC0bits.T1IE = 1;
				setPWM(0);
			}
		}
		else
		{
			SSD_status = false;
			if(dip_status & 0x1) // 11
			{
				// do nothing
			}
			else
			{
				IEC0bits.T1IE = 0;
				setPWM(led_brightness[((PORTE >> 6) & 0x3)]);
			}
		}
	}
	return 0;
}
