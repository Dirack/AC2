#include <detpic32.h>
void delay(unsigned int);
unsigned char toBcd(unsigned char);
void send2displays (unsigned char);
void configureAll(void);

volatile unsigned char value2display = 0; //Variavel Global

int main(void)
{
	configureAll();
	IFS1bits.AD1IF = 0;		//Reset Flag AD1
	IFS0bits.T1IF=0;       		//Reset Flag T1
	IFS0bits.T3IF=0;       		//Reset Flag T3
	EnableInterrupts();    		//Ativaçao Global
	while(1);
		
}
void configureAll(void)
{
	TRISB = 0xFC00;
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
		
	IEC1bits.AD1IE = 1;		// Activação Parcial
	IPC6bits.AD1IP = 1;		// Prioridade Partecial
//
	T1CONbits.TCKPS=3;
	PR1=19530;
	TMR1=0;
	T1CONbits.TON=1;

	IFS0bits.T1IF=0;       		//Reset 
	IEC0bits.T1IE=1;		//Ativaçao Parcial 	
	IPC1bits.T1IP=1;		//T3IP
//
	T3CONbits.TCKPS=3;
	PR3=24999;
	TMR3=0;
	T3CONbits.TON=1;    		//Enable

	IFS0bits.T3IF=0;       		//Reset 
	IEC0bits.T3IE=1;		//Ativaçao Parcial 	
	IPC3bits.T3IP=1;		//T3IP

}
void _int_(4) isr_t1(void)
{
	AD1CON1bits.ASAM = 1; 		// Start Conversion
	IFS0bits.T1IF=0; 
}
void _int_(12) isr_t3(void)
{
	char value = toBcd(value2display);
	send2displays(value);
	IFS0bits.T3IF=0;
}
void _int_(27) isr_adc(void)
{
	double soma=0,media=0;
	int *p = (int *)(&ADC1BUF0);
	int i = 0;
	for(i = 0; i < 8; i++)
	{
		soma += (p[i*4]);
	}		
	media = soma / 8;
	value2display = (media*33)/1023;
	i=0;
	IFS1bits.AD1IF = 0;		// Reset flag
}

unsigned char toBcd(unsigned char value)
{
	return ((value / 10) << 4) + (value % 10);
}
void send2displays(unsigned char value)
{
	static unsigned char displayFlag = 0;
	static const unsigned char display7Scodes[] = {0x77,0x41,0x3b,0x6b,0x4d,0x6e,0x7e,0x43,0x7f,0x4f,0x5f,0x7c,0x36,0x79,0x3e,0x1e,}; //{0,1,2,3,4,5,6,7,8,9,A,b,C,d,E,F}
	
	int a,b;
	a = value % 10;
	b = (value-a) / 10;
	
	if(displayFlag==0)
	{
		LATB = 0x100 + display7Scodes[a];
	}
	else
	{
		LATB = 0x200 + display7Scodes[b];
	}
		displayFlag = ~displayFlag; //Altera o displayFlag entre 0 e 1
}

