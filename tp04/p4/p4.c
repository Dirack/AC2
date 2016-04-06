#include <detpic32.h>
#include <ptrodrigues.h>

void __init__()
{
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	TRISBbits.TRISB14 	= 1;
	AD1PCFGbits.PCFG14	= 0;
	AD1CHSbits.CH0SA	= 14;
	AD1CON2bits.SMPI	= 3;	// 4 Sample taken
	AD1CON1bits.ON		= 1;
}

int main(void)
{
	__init__();
	int i;
	while(true)
	{
		int *p = (int *)(&ADC1BUF0);
		AD1CON1bits.ASAM = 1;
		while( IFS1bits.AD1IF == 0 );
		for(i = 0; i < 16; i++)
		{
			printInt(p[i*4],(4<<16) + 10);
			putChar(' ');
		}
		putChar('\n');
		IFS1bits.AD1IF = 0;
	}
	return 0;
}
