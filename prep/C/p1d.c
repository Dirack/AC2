#include <detpic32.h>
#define CALIBRATION_VALUE 4998
#define ADCsamples 1

typedef enum{false,true} boolean;
typedef unsigned int uint;
typedef unsigned char uchar;
enum SSD_segments{
        b = 0x01,
        a = 0x02,
        f = 0x04,
        g = 0x08,
        e = 0x10,
        d = 0x20,
        c = 0x40,
        dp= 0x80
};

void delay(uint n_intervals)
{
        volatile uint i;

        for(; n_intervals != 0; n_intervals--)
                for(i = CALIBRATION_VALUE; i != 0; i--)
                ;
}


void _init_(void)
{
	TRISB = TRISB & 0xFC00;
	LATB = (LATB & 0xFC00);
}

void _initADC_(uint nSamples)
{
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0;
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = (nSamples - 1);
	AD1CHSbits.CH0SA = 14;
	AD1CON1bits.ON = 1;
	IPC6bits.AD1IP = 1;
	IEC1bits.AD1IE = 1;
	IFS1bits.AD1IF = 0;
}

void send2SSD(uchar value,boolean high_display)
{

	LATB = ((LATB & 0xFC00) | (high_display ? 0x100 : 0x200) ) | value;
}

volatile uint ADCvalue = 1;

int main(void)
{
	static uchar segments[]={f,a,b,e,d,c,b,a,f,c,d,e};
	_init_();
	_initADC_(ADCsamples);
	EnableInterrupts();
	int j = 0;
	AD1CON1bits.ASAM = 1;
	while(true)
	{
		send2SSD(segments[j],(j < 3 || j > 8) ? true : false );
		delay(250*ADCvalue);
		if(++j == 12)
			j = 0;
	}

	return 0;
}

void _int_(27) isr_adc(void)
{
	uint i;
	uint *ADCpointer = (uint *)&ADC1BUF0;
	uint ADCmean = 0;
	for(i = 0; i < ADCsamples; i++, ADCpointer = ADCpointer + 4)
	{
		ADCmean += *ADCpointer;
	}
	ADCvalue = (((ADCmean/ADCsamples) * 7) / 1023) + 1;
	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
}
