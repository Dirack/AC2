#include "detpic32.h"
char ler(void);
void escrever(char);
void main(void)
{
	TRISB = TRISB & 0xFC00;
	LATBbits.LATB9 = 1;
	LATBbits.LATB8 = 1;

	while(1)
	{
	char c = ler();
	escrever(c);
	}
	
}
char ler()
{
	char c = getChar();
	if(c=='a'||c=='b'||c=='c'||c=='d'||c=='e'||c=='f'||c=='g'||c=='.'||c=='A'||c=='B'||c=='C'||c=='D'||c=='E'||c=='F'||c=='G')
	{	
		return c;
	}
	else
	{	printStr("Caracter errado!\n");
		ler();
	}
}
void escrever(char c)
{
	LATB = LATB & 0xFF00;
	switch(c)
	{
		case 'a':
			LATBbits.LATB1 = 1;	
			break;
		case 'b':
			LATBbits.LATB0 = 1;
			break;
		case 'c':
			LATBbits.LATB6 = 1;
			break;
		case 'd':
			LATBbits.LATB5 = 1;
			break;
		case 'e':
			LATBbits.LATB4 = 1;
			break;
		case 'f':
			LATBbits.LATB2 = 1;
			break;
		case 'g':
			LATBbits.LATB3 = 1;
			break;
		case 'A':
			LATBbits.LATB1 = 1;
			break;
		case 'B':
			LATBbits.LATB0 = 1;
			break;
		case 'C':
			LATBbits.LATB6 = 1;
			break;
		case 'D':
			LATBbits.LATB5 = 1;
			break;
		case 'E':
			LATBbits.LATB4 = 1;
			break;
		case 'F':
			LATBbits.LATB2 = 1;
			break;
		case 'G':
			LATBbits.LATB3 = 1;
			break;
		case '.':
			LATBbits.LATB7 = 1;
			break;
	}
}
