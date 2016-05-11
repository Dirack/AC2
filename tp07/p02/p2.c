#include <detpic32.h>
#include <ptrodrigues.h>

#define uart_baud 115200//baudrate
#define uart_par	//parity
#define uart_dbits 8	//data bits
#define uart_sbits 1	//stop bits

void _init(void)
{
	U1BRG = ((PBCLK + 8 * uart_baud) / (16 * uart_baud)) - 1; // gerador baudrate
	U1MODEbits.BRGH = 0;
	U1MODEbits.PDSEL = 0;
	U1MODEbits.STSEL = 0;
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;
}

void putc(char byte2send)
{
	while(U1STAbits.UTXBF == 1);
	U1TXREG = byte2send;
}

int main(void)
{
	_init();
	while(true)
	{
		putc('+');
		delay(1000);
	}
	return 0;
}
