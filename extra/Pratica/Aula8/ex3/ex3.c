#include <detpic32.h>
void putc(char byte2send)
{
	// wait while UTXBF == 1
	while(	U1STAbits.UTXBF == 1);
	// Copy byte2send to the UxTXREG register
	U1TXREG = byte2send;
}
void puts(char *str)
{
	int i = 0;
	while (str[i] != '\0'){
		putc(str[i]);
		i++;
	}
}

void main(void)
{
	// Configure UART1:
	// 1 - Configure BaudRate Generator
	U1MODEbits.BRGH = 0;
	U1BRG = 10;
	// 2 ¿ Configure number of data bits, parity and number of stop bits
	//(see U1MODE register)

	U1MODEbits.PDSEL = 0;
	U1MODEbits.STSEL = 0;
		// 3 ¿ Enable the trasmitter and receiver modules (see register U1STA)
		U1STAbits.UTXEN = 1;
		U1STAbits.URXEN = 1;
		// 4 ¿ Enable UART1 (see register U1MODE)
		U1MODEbits.ON;
	while(1){
		puts("String de teste \n");
		resetCoreTimer();
		int timer = readCoreTimer();
		while(timer <= 20000000){
			timer = readCoreTimer();
		}
	}
}
