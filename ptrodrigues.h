#define CALIBRATION_VALUE 4998
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

/*
static uchar segment_array[] = {a,b,c,d,e,f,g,dp,0x00};
static uchar char_array[] = {
	ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,
	A,B,C,D,E,F,DP,0x00};
*/

uint set_SSD_Hex(uint port, uchar character, boolean high_SSD)
{
	return (port & 0xFC00) | (character | (high_SSD ? 0x0100 : 0x0200));
}


void delay(uint n_intervals)
{
	volatile uint i;

	for(; n_intervals != 0; n_intervals--)
		for(i = CALIBRATION_VALUE; i != 0; i--)
		;
}

void init_adc14(uint n_samples)
{
	AD1CON1bits.SSRC = 7;			// Start conversion after delay
	AD1CON1bits.CLRASAM = 1;		// Stop conversion after 1st interrupt
	AD1CON3bits.SAMC = 16;			// Sample time 16*100ns = 16us
	TRISBbits.TRISB14 	= 1;		// RB14 as INPUT
	AD1PCFGbits.PCFG14	= 0;		// RB14 as ANALOG
	AD1CHSbits.CH0SA	= 14;		// Select AN14 in mux
	AD1CON2bits.SMPI	= n_samples -1;	// 4 Sample taken
	AD1CON1bits.ON		= 1;		// Enable ADC
}
