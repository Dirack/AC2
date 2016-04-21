#include <detpic32.h>
#include <ptrodrigues.h>
#define FREQT1 50	// ADC refresh rate
#define FREQT3 100
#define preT1 3
#define preT3 2
#define dc 25
#define ADCSAMPLES 4

volatile uint ADCvalue = 0;
// RD0 OC1
// Freq 100Hz
// Duty-Cycle 25%
// Base tempo T3

void _init_(uint nSamples)
{	
	init_adc14(nSamples);
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
	
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	OC1RS = ((PR3+1) * dc) / 100;
	OC1CONbits.ON = 1;
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
	while(true)
	{
		setPWM(ADCvalue);
	}
	return 0;
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
	 ADCvalue = ((mean / ADCSAMPLES)*100)/1023;
	 IFS1bits.AD1IF = 0;
	 IFS0bits.T1IF = 0;
}
