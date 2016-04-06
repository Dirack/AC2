#include <detpic32.h>
char getc(void)
{
	// If OERR == 1 then reset OERR
	if(U1STAbits.OERR == 1) U1STAbits.OERR = 0;
	// Wait while URXDA == 0
	while(U1STAbits.URXDA == 0);
	// If FERR or PERR then read UxRXREG and return 0
	if(U1STAbits.FERR == 1 || U1STAbits.PERR == 1){
		U1RXREG;
		return 0;
	}
	// Return U1RXREG
	return U1RXREG;
}
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
void configUart(unsigned int baud, char parity, unsigned int stopbits)
{
	// Configure BaudRate Generator
	if(baud >= 600 && baud <= 115200){
		U1MODEbits.BRGH = 0;
		U1BRG = (20000000+(baud*8))/(baud*16)-1;
	}
	else{
		U1MODEbits.BRGH = 0;
		U1BRG = 10;
	}
	// Configure number of data bits (8), parity and number of stop bits
	if(parity == 'N'){
		U1MODEbits.PDSEL = 0;
	}
	else if(parity == 'E'){
		U1MODEbits.PDSEL = 1;
	}
	else if(parity == 'O'){
		U1MODEbits.PDSEL = 2;
	}
	else U1MODEbits.PDSEL = 0;

	if(stopbits == 1){
		U1MODEbits.STSEL = 0;
	}
	else if(stopbits == 2){
		U1MODEbits.STSEL = 1;
	}
	else U1MODEbits.STSEL = 0;
	// Enable the trasmitter and receiver modules
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	// Enable UART1
	U1MODEbits.ON;
}
int main(void)
{
	configUart(115200,'N',1);
	while(1)
	{
		putc( getc() );
	}
}
// default "pterm" parameters

