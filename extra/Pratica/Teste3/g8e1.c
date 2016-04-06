#include <detpic32.h>

void putc(char byte2send){
	while(U1STAbits.UTXBF == 1);
	U1TXREG = byte2send;
}
int main(void)
{
	//BaudRate Config: 
	U1MODEbits.BRGH = 0; // /16
	U1BRG = 10;

	U1MODEbits.PDSEL = 0; //8 data bits, no parity
	U1MODEbits.STSEL = 1; //1 stop bits

	U1STAbits.UTXEN = 1; //transmission module enabled
	U1STAbits.URXEN = 1; //reception module enabled

	U1MODEbits.ON = 1; //UART enabled 

	while(1){
		putc('+');
		resetCoreTimer();
		while(readCoreTimer() <= 20000000);
	}	
}

