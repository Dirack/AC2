#include <detpic32.h>
#include <ptrodrigues.h>

#define uart_baud 115200//baudrate
#define uart_par	//parity
#define uart_dbits 8	//data bits
#define uart_sbits 1	//stop bits

void putc(char byte2send)
{
	while(U1STAbits.UTXBF == 1);
	U1TXREG = byte2send;
}

void puts(char *str)
{
	do{
		putc(*str);
	}while(*++str != '\0');
}

char UART_decodeParity(char parity)
{
	switch(parity)
	{
		case 'N':
			return	0x0;
		case 'E':
			return 	0x1;
		case 'O':
			return	0x2;
		default:
			return 0xFF;
	}
}

boolean UART_checkArgs(uint baud, char parity, uint stopbits)
{
	boolean check = true;
	if(baud < 600 || baud > 115200)
	{
		puts("\nUART1 ERROR: 600 < Baudrate > 115200");
		check = false;
	}
	if(UART_decodeParity(parity) == -1)
	{	puts("\nUART1 ERROR: Parity: 'N', 'E', 'O'");
		check = false;
	}
	if(stopbits > 2)
	{
		puts("\nUART1 ERROR: Stop bits: 1/2");
		check = false;
	}
	putc('\n');
	return check;
}

void UART1_PrintCfg(uint baud, char parity, uint stopbits)
{
	U1STAbits.UTXBRK = 1;
	puts("//////// UART1 configuration ////////\nBaudrate: ");
	printInt10(baud);
	puts("\nParity: ");
	putc(parity);
	puts("\nStop bits: ");
	printInt10(stopbits);
	puts("\n/////////////////////////////////////\n");
}

void configUart(uint baud, char parity, uint stopbits)
{
	if(!UART_checkArgs(baud, parity, stopbits))
	{
		baud = 115200;
		parity = 'N';
		stopbits = 1;
	}
	
	U1BRG = ((PBCLK + 8 * baud) / (16 * baud)) - 1; // gerador baudrate
	U1MODEbits.BRGH = 0;
	U1MODEbits.PDSEL = UART_decodeParity(parity);
	U1MODEbits.STSEL = stopbits-1;
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;
	UART1_PrintCfg(baud, parity, stopbits);
}

int main(void)
{
	U1STAbits.UTXBRK = 1;
	configUart(1200,'O',2);
	while(true)
	{
		puts("String test\n");
		delay(1000);
	}
	return 0;
}
