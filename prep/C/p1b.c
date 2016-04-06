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
}

void send2SSD(uchar value)
{
	LATB = ((LATB & 0xFC00) | 0x100 ) | value;
}

int main(void)
{
	static uchar segments[]={f,a,b,g,e,d,c,g};
	_init_();
	_initADC_(ADCsamples);
	int j = 0;
	int ADCvalue = 0;
	while(true)
	{
		AD1CON1bits.ASAM = 1;
		while(IFS1bits.AD1IF == 0);
		uint i;
		uint *ADCpointer = (uint *)&ADC1BUF0;
		for(i = 0; i < ADCsamples; i++, ADCpointer = ADCpointer + 4)
		{
			ADCvalue += *ADCpointer;
		}
		ADCvalue = (((ADCvalue/ADCsamples) * 7) / 1023) + 1;
		send2SSD(segments[j]);
		uint timer = 250*ADCvalue;
		delay(timer);
		if(++j == 8)
			j = 0;
		IFS1bits.AD1IF == 0;
	}

	return 0;
}
