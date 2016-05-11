#include <detpic32.h>
#include <ptrodrigues.h>
#define ADC_nsamples 4
void __init__()
{
	init_adc14(ADC_nsamples);
	TRISB &= 0xFC00;			// RB0 to RB9 as INPUT
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

uchar toBCD(uchar character)
{
	return (((character/10)<<4) + (character % 10));
}

int main(void)
{
	__init__();
	int i;
	while(true)
	{
		i = 0;
		uchar value = toBCD(0);
		do{
			if((i++ % 25) == 0) // 250ms
			{	

				int V_mean = 0;
				int *p = (int *)(&ADC1BUF0);
				AD1CON1bits.ASAM = 1;
				while( IFS1bits.AD1IF == 0 );
				int j;
				for(j = 0; j < ADC_nsamples; j++)
				{
					V_mean += p[j*4];
				}
				value = toBCD(((V_mean/ADC_nsamples)*33)/1023);
				printInt(value >> 4, 10);
				putChar('.');
				printInt(value & 0xF, 10);
				putChar('\n');
				IFS1bits.AD1IF = 0;
			}
			send2SSD(value);
			delay(10);
		}while(i < 1000);
	}
	return 0;
}
