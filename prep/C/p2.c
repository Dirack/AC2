#define CALIBRATION_VALUE 4998
#include <detpic32.h>
#define ADCSAMPLES 1
typedef unsigned int uint;
typedef unsigned char uchar;
typedef enum{false,true} boolean;
enum _SSD_segments{
	b = 0x01,
	a = 0x02,
	f = 0x04,
	g = 0x08,
	e = 0x10,
	d = 0x20,
	c = 0x40,
	dp= 0x80
};

enum _SSD_hex{
	A = a|b|c|e|f|g,
	B = c|d|e|f|g,
	C = a|d|e|f,
	D = b|c|d|e|g,
	E = a|d|e|f|g,
	F = a|e|f|g,
	DP= dp,
	ZERO = C|b|c,
	ONE  = b|c,
	TWO  = a|b|d|e|g,
	THREE= ONE|a|d|g,
	FOUR = ONE|f|g,
	FIVE = a|c|d|f|g,
	SIX  = B|a,
	SEVEN= ONE|a,
	EIGHT= ZERO|g,
	NINE = FIVE|b,
	CHECK= EIGHT|dp
};
const static uchar SSD_decode[] = {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE};

void delay(unsigned int n_intervals)
{
volatile unsigned int i;
	for(; n_intervals != 0; n_intervals--)
		for(i = CALIBRATION_VALUE; i != 0; i--)
		;
}

void _init(nsamples)
{
	TRISB = TRISB & 0xFC00;
	//TRISB = (TRISB & 0x00FF);	//RB15 - RB8 SSD's / RB4 - ADC INPUT
	//TRISD = (TRISD & 0xFF9F); 	//RD5 - LD / RD6 - HD
	AD1PCFGbits.PCFG14 = 0;
	//AD1PCFGbits.PCFG4 = 0;
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = nsamples - 1;
	AD1CHSbits.CH0SA = 14;
	//AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;
}

uchar bcd(uint val)
{
	return (((val / 10) << 4) + val % 10);
}

void send2display(uchar val)
{
	static boolean high = false;
	//LATD = (LATD & 0xFF9F) | (high ? 0x40:0x20);
	LATB = (LATB & 0xFC00) | (high ? 0x100:0x200);
	//LATB = (LATB & 0x00FF) | (high ? (SSD_decode[val >> 4] << 8):(SSD_decode[(val & 0x0F)] << 8));
	LATB = LATB | (high ? (SSD_decode[val >> 4]):(SSD_decode[(val & 0x0F)]));
	high ^= 0x1;
}

int main(void)
{
	_init(ADCSAMPLES);
	uint ADCval = 0;
	while(true)
	{
		uint i = 0;
		do{
		delay(5);
		if(i % 2 == 0)
		{
			AD1CON1bits.ASAM = 1;
			while(IFS1bits.AD1IF == 0);
		}
		ADCval = (ADC1BUF0 * 70)/1023;
		send2display(bcd(ADCval));
		IFS1bits.AD1IF = 0;
		}while(++i < 200);
	}
	return 0;
}
