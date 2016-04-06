#include <detpic32.h>
int main(void)
{
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

	IFS1bits.AD1IF = 0;		// Reset flag

	EnableInterrupts();		// Activação Geral	
		
	IEC1bits.AD1IE = 1;		// Activação Parcial
	IPC6bits.AD1IP = 1;		// Prioridade Partecial

	AD1CON1bits.ASAM = 1; 		// Start Conversion

	while(1);
	return 0;

}
void _int_(27) isr_adc(void)
{	
	LATEbits.LATE0 = 0;
	int aux = ADC1BUF0;
	//printInt(ADC1BUF0,0x00030010);	// Imprimir hexadecimal com 3 algarismos
	//printf("\n");
	LATEbits.LATE0 = 1;
	AD1CON1bits.ASAM = 1; 		// Start Conversion
	IFS1bits.AD1IF = 0;		// Reset flag
}	

//4.4 micro-segundos - Tempo de Conversão + Tempo de Latencia
//3.6 micro-segundos - Tempo de Conversão (a05e03)
//0.8 micro-segundos - Tempo de Latencia
