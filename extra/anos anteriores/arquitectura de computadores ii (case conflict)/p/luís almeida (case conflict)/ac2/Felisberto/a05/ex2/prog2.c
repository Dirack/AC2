#include <detpic32.h>

int main(void)
{
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
		AD1CON1bits.ASAM = 1; 		// Start Conversion
		while( IFS1bits.AD1IF == 0 ); 	// Wait while conversion not done
		printInt(ADC1BUF0,16);
		printf("\n");
		IFS1bits.AD1IF = 0;		//Reset AD1IF
	}

	return 0;
	
}
