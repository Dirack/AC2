#include <detpic32.h>
#include <ptrodrigues.h>
// By observation in osciloscope conversion was made in +/- 3.5uS ;
void __init__()
{
	TRISEbits.TRISE0 = 0;
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	TRISBbits.TRISB14 	= 1;
	AD1PCFGbits.PCFG14	= 0;
	AD1CHSbits.CH0SA	= 14;
	AD1CON2bits.SMPI	= 0;	// 1 Sample taken
	AD1CON1bits.ON		= 1;
}

int main(void)
{
	__init__();
	volatile int aux;
	while(true)
	{
		LATEbits.LATE0	= 1;
		AD1CON1bits.ASAM = 1;
		while( IFS1bits.AD1IF == 0 );
		LATEbits.LATE0	= 0;
		aux = ADC1BUF0;
		IFS1bits.AD1IF = 0;
	}
	return 0;
}
