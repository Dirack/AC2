#include <detpic32.h>
void delay(unsigned int);
unsigned char toBcd(unsigned char);
void send2displays (unsigned char);

int main(void)
{

	//FASE 1
	TRISBbits.TRISB14 = 1; 		//Desligar entrada digital
	AD1PCFGbits.PCFG14 = 0; 	//Configurar como entrada analogica

	//FASE 2
	AD1CHSbits.CH0SA = 14;		//Valor passado ao select do mux para por o valor de RB14 na saída
	//FASE 3
	AD1CON2bits.SMPI = 3;		// Numero de conversões = valor + 1;

	//FASE 4
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON1bits.ON = 1;
	
	int *p = (int *)(&ADC1BUF0);
	int i = 0;
	int soma = 0;
	double media = 0,v=0;
	char n;
	while(1)
	{
		delay(10);
		if(i++ == 25) //250ms
		{ 
			AD1CON1bits.ASAM = 1; 		// Start Conversion
			while( IFS1bits.AD1IF == 0 ); 	// Wait while conversion not done
	
			for(i = 0; i < 16; i++)
			{
				soma += (p[i*4]);
			}
			media = soma / 4;
			v = (media*33)/1023;
			n = toBcd(v);
			i=0;
			IFS1bits.AD1IF = 0;		// Reset AD1IF
		}
		send2displays(n);
	}
	return 0;
	
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
void send2displays (unsigned char value)
{
	TRISB = (TRISB & 0xFC00);
	unsigned char low, high;
	static unsigned char flag;
	static const unsigned char codes[] = {0x77, 0x41, 0x3b, 0x6b, 0x4d, 0x6e, 0x7e, 0x43, 0x7f, 0x6f, 0x5f, 0x7c, 0x36, 0x79, 0x3e, 0x1e};
	if (flag == 1)
	{	LATBbits.LATB8 = 1;
		LATBbits.LATB9 = 0;
		LATBbits.LATB7 = 1;
	
		
		high = (value & 0xF0);
		if (high == 0x00)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[0]);}
		if (high == 0x10)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[1]);}
		if (high == 0x20)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[2]);}
		if (high == 0x30)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[3]);}
		if (high == 0x40)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[4]);}
		if (high == 0x50)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[5]);}
		if (high == 0x60)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[6]);}
		if (high == 0x70)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[7]);}
		if (high == 0x80)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[8]);}
		if (high == 0x90)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[9]);}
		if (high == 0xa0)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[10]);}
		if (high == 0xb0)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[11]);}
		if (high == 0xc0)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[12]);}
		if (high == 0xd0)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[13]);}
		if (high == 0xe0)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[14]);}
		if (high == 0xf0)
			{LATB = (LATB & 0xFF80);
			LATB = (LATB | codes[15]);}
		flag = 0;
		return;
		}
		if (flag == 0)
		{
		LATBbits.LATB8 = 0;
		LATBbits.LATB9 = 1;
		low = (value & 0x0F);
		if (low == 0x00)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[0]);}
		if (low == 0x01)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[1]);}
		if (low == 0x02)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[2]);}
		if (low == 0x03)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[3]);}
		if (low == 0x04)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[4]);}
		if (low == 0x05)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[5]);}
		if (low == 0x06)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[6]);}
		if (low == 0x07)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[7]);}
		if (low == 0x08)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[8]);}
		if (low == 0x09)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[9]);}
		if (low == 0x0a)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[10]);}
		if (low == 0x0b)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[11]);}
		if (low == 0x0c)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[12]);}
		if (low == 0x0d)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[13]);}
		if (low == 0x0e)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[14]);}
		if (low == 0x0f)
			{LATB = (LATB & 0xFF00);
			LATB = (LATB | codes[15]);}
		flag = 1;
		}
}
