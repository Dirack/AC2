#include <detpic32.h>

char toBdc(unsigned char value);
void delay(unsigned int n_intervals);
void send2displays(unsigned char value);

int main(void)
{	
	TRISB = 0xFC00; //1111 1100 0000 0000 Configura todos os portos como saída	
	while(1)
	{
		char value = 0b0000000; // Valor inicial
		while(value <= 0b1100011) // Valor <= 99
		{
			int i = 0;
			do{
				delay(50); //ex11 - 10
				send2displays(value);
			}while(++i < 4); //ex11 - 20
			value += 0b00000001; //incremento
		}
	}
	return 0;
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
		PORTB = 0x100 + display7Scodes[a];
	}
	else
	{
		PORTB = 0x200 + display7Scodes[b];
	}
		displayFlag = ~displayFlag; //Altera o displayFlag entre 0 e 1
}


void delay(unsigned int n_intervals)
{
	volatile unsigned int i;
	for(; n_intervals != 0; n_intervals--)
	{
		for(i = 4997; i != 0; i--);
	}
}
unsigned char toBcd(unsigned char value)
{
	return ((value / 10) << 4) + (value % 10);
}
