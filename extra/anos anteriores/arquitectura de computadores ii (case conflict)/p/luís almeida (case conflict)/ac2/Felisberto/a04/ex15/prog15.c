#include <detpic32.h>

char toBdc(unsigned char value);
void delay(unsigned int n_intervals);
void send2displays(unsigned char value);

int main(void)
{	
	TRISB = 0xFC00; //1111 1100 0000 0000 Configura todos os portos como saída
	int volta = 0;	
	while(1)
	{
		char value = 0b0000000; // Valor inicial
		if(volta!=0)
		{
			int k = 0;
			do{
				int j = 0;
				do{
					delay(5);
					send2displays(value);
				}while(++j<50);

				j = 0;

				do{
					delay(5);
					PORTB = 0x000;
				}while(++j<50);
			}while(++k<10);
		}
		while(value <= 0b1100011) // Valor <= 99
		{
			int i = 0;
			do{
				delay(5);	//Partimos o delay de 10 para duas vezes o 5 pois de uma vez acende os numeros de outra o ponto
				send2displays(value);
				delay(5);
				if(value%2==0)
				{
					PORTB = 0x0180; //0000 0010 1000 0000	
				}
				else
				{
					PORTB = 0x0280; //0000 0001 1000 0000
				}
				
			}while(++i < 100);
			value += 0b00000001; //incremento
		}
		volta=1;
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
