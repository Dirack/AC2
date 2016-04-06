#include <detpic32.h>

int main(void)
{
	volatile int aux;
	//Configurar como saída RE0
	TRISEbits.TRISE0 = 0;

	//FASE 1
	TRISBbits.TRISB14 = 1; 		//Desligar entrada digital
	AD1PCFGbits.PCFG14 = 0; 	//Configurar como entrada analogica

	//FASE 2
	AD1CHSbits.CH0SA = 14;		//Valor passado ao select do mux para por o valor de RB14 na saída
	//FASE 3
	AD1CON2bits.SMPI = 0;

	//FASE 4
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON1bits.ON = 1;
	
	while(1)
	{
		LATEbits.LATE0 = 1;		// SET RE0
		AD1CON1bits.ASAM = 1; 		// Start Conversion
		while( IFS1bits.AD1IF == 0 ); 	// Wait while conversion not done
		LATEbits.LATE0 = 0;		// Reset RE0
		aux = ADC1BUF0;			// Read conversion result (ADC1BUF0) to "aux" variable
		IFS1bits.AD1IF = 0;		// Reset AD1IF
	}

	return 0;

	// Tempo = 3.6 us (microssegundos)
	
}
