#include <detpic32.h>
void delay(unsigned int);
unsigned char toBcd(unsigned char);
void send2displays (unsigned char);
volatile unsigned char value2dysplay = 0; //Variavel Global

int main(void)
{
	//FASE 1
	TRISBbits.TRISB14 = 1; 		//Desligar entrada digital
	AD1PCFGbits.PCFG14 = 0; 	//Configurar como entrada analogica

	//FASE 2
	AD1CHSbits.CH0SA = 14;		//Valor passado ao select do mux para por o valor de RB14 na saída
	//FASE 3
	AD1CON2bits.SMPI = 7;		// Numero de conversões = valor + 1;

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
	
	int i = 0;
	while(1)
	{
		delay(10);
		if(i++ == 25) //250ms
		{ 
			AD1CON1bits.ASAM = 1; 		// Start Conversion
			i = 0;
		}
		send2displays(value2dysplay);
	}
	return 0;
	
}

void _int_(27) isr_adc(void)
{
	double soma=0,media=0,v=0;
	int *p = (int *)(&ADC1BUF0);
	int i = 0;
	//printInt(ADC1BUF0,0x00030010);	// Imprimir hexadecimal com 3 algarismos
	//printf("\n");
	for(i = 0; i < 8; i++)
	{
		soma += (p[i*4]);
	}		
	media = soma / 8;
	v = (media*33)/1023;
	value2dysplay  = toBcd(v);
	i=0;

	IFS1bits.AD1IF = 0;		// Reset flag
}	
void delay(unsigned int n_intervals)
{
	volatile unsigned int i;
	for(; n_intervals != 0; n_intervals--);
	for(i = 4997; i != 0; i--);
}
unsigned char toBcd(unsigned char value)
{
	return ((value / 10) << 4) + (value % 10);
}
void send2displays(unsigned char value){
  static unsigned char displayFlag = 0;
  static const unsigned char display7codes [] = {
    0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E, 0x7E
  };

  int a, b;
  a = value % 10;
  b = (value - a)/10;

  if (displayFlag == 0){
    PORTB = 0x0200 + display7codes[a];
  }else{
    /* adicionei 0x80 para aparecer sempre o ponto quando é imprimido
    o valor do select high... Por alguma razao que não percebi, se
    fizesse um ciclo externo para imprimir o ponto fora desta funcao, o
    a tensao maxima de 3.3 nunca aparecia... dava sempre 3.2 e se 
    tentar neste momento meter a zero, o valor minimo que obtenho 
    é 0020 no terminal, nas no arredondamento dos displays aparece 0.0,
    neste caso nao é preocupante, mas se tivesse mais displays para mostrar
    as unidades centesimas e milesimas já teria problemas de novo... 
    nao percebo porque razao isto acontece...*/
    PORTB = 0x0100 + display7codes[b] + 0x80;
  }
  displayFlag = ~displayFlag;
}
