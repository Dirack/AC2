#include <detpic32.h>

void main(void)
{
	TRISB = TRISB & 0xFC00; 	//RB0-9 -> Output
	LATBbits.LATB9 = 1;
	LATBbits.LATB8 = 1;

	while(1){
		char c ;
		printStr("\nInsira um caracter: ");
		c = getChar();	
		putChar(c);
		LATB = LATB & 0xFf00;
		if((c == 'a') || (c == 'A'))
			LATBbits.LATB1 = 1;
		if(c == 'b' || c == 'B')
			LATBbits.LATB0 = 1;
		if(c == 'c' || c == 'C')
			LATBbits.LATB6 = 1;
		if(c == 'd' || c == 'D')
			LATBbits.LATB5 = 1;
		if(c == 'e' || c == 'E')
			LATBbits.LATB4 = 1;
		if(c == 'f' || c == 'F')
			LATBbits.LATB2 = 1;
		if(c == 'g' || c == 'G')
			LATBbits.LATB3 = 1;
		if(c == '.')
			LATBbits.LATB7 = 1;
		
	}
}

