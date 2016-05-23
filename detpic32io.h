/*	DETPIC32 IO Configuration
 ******* DipSwitch
 * 	DipSwitch 	RB0/1/2/3
 ******* ADC
 * 	ADC			RB4
 * 
 ******* 7SegmentsDisplay
 * 	7SegDISP_A	RB8
 * 	7SegDISP_B	RB9
 * 	7SegDISP_C	RB10
 * 	7SegDISP_D	RB11
 * 	7SegDISP_E	RB12
 * 	7SegDISP_F	RB13
 * 	7SegDISP_G	RB14
 *  DISP_LOW	RD5
 *  DISP_HIGH	RD6
 ******* LEDs
 *  LED[0:7]	RE0...RE7
 ******* UART
 * 	UART COMMUNICATION OVER UART2
 * 
 * */

//******* Configuração de portas
//******* DipSwitch, ADC & 7SegDISP
TRISB = (TRISB & 0x80F0);
TRISD = (TRISD & 0xFF9F);
//******* LEDs
TRISE = (TRISE & 0XFF00);

unsigned char toBCD(unsigned char value)
{
	return ((value/10) << 4) + value%10;
}

void send2displays(unsigned char value)
{
	unsigned char DECODE_ARRAY[] = {0,1,2,3,4,...,F};
	static unsigned char high_disp = 0;
	if(high_disp)
	{
		LATB = (LATB & 0x80FF) | (DECODE_ARRAY[(toBCD(value) >> 4)] << 8);
		LATD = (LATD & 0xFF9F) | 0x40;
	}
	else
	{
		LATB = (LATB & 0x80FF) | ((toBCD(value) & 0x0F) << 8);
		LATD = (LATD & 0xFF9F) | 0x20;
	}
	high_disp ^= 1;
}










