#include <detpic32.h>
#include <ptrodrigues.h>


#define uart_baud 115200// UART1 baudrate
#define uart_par 'N'	// UART1 parity
#define uart_dbits 8	// UART1 data bits
#define uart_sbits 1	// UART1 stop bits
#define UART1_IP 2


#define DisableUart1RxInterrupt()	IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt()	IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt()	IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt()	IEC0bits.U1TXIE = 1
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)

typedef struct{
	uchar	data[BUF_SIZE];
	uint	head;
	uint	tail;
	uint 	count;
} circularBuffer;

volatile circularBuffer txb;
volatile circularBuffer rxb;


void comDrv_flushRx(void)
{
	rxb.head = 0;
	rxb.tail = 0;
	rxb.count = 0;
}

void comDrv_flushTx(void)
{
	txb.head = 0;
	txb.tail = 0;
	txb.count = 0;
}

void comDrv_putc(char ch)
{
	while(txb.count == BUF_SIZE);
	txb.data[txb.tail] = ch;
	txb.tail = (txb.tail + 1) & INDEX_MASK;
	DisableUart1TxInterrupt();
	txb.count++;
	EnableUart1TxInterrupt();
}

void comDrv_puts(char *s)
{
	while(*s != '\0')
	{
		comDrv_putc(*s++);
	}
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
		//comDrv_puts("\nUART1 ERROR: 600 < Baudrate > 115200");
		check = false;
	}
	if(UART_decodeParity(parity) == -1)
	{	//comDrv_puts("\nUART1 ERROR: Parity: 'N', 'E', 'O'");
		check = false;
	}
	if(stopbits > 2)
	{
		//comDrv_puts("\nUART1 ERROR: Stop bits: 1/2");
		check = false;
	}
	//comDrv_putc('\n');
	return check;
}

void UART1_PrintCfg(uint baud, char parity, uint stopbits)
{
	U1STAbits.UTXBRK = 1;
	printStr("//////// UART1 configuration ////////\nBaudrate: ");
	printInt10(baud);
	printStr("\nParity: ");
	putChar(parity);
	printStr("\nStop bits: ");
	printInt10(stopbits);
	printStr("\n/////////////////////////////////////\n");
}

void comDrv_config(uint baud, char parity, uint stopbits)
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
	
	U1STAbits.URXISEL = 0;
	IFS0bits.U1RXIF = 0;
	IEC0bits.U1RXIE = 1;
	IFS0bits.U1TXIF = 0;
	IEC0bits.U1TXIE = 1;
	IEC0bits.U1EIE = 0;
	IPC6bits.U1IP = UART1_IP;
	U1MODEbits.ON = 1;
	UART1_PrintCfg(baud, parity, stopbits);
}

int main(void)
{
	comDrv_config(115200,'N',1);
	comDrv_flushRx();
	comDrv_flushTx();
	EnableInterrupts();
	
	
	while(true)
	{
		comDrv_puts("Pedro Rodrigues 71990\n");
	}
	return 0;
}

void _int_(24) isr_uart1(void)
{
	if(IFS0bits.U1TXIF)
	{
		if(txb.count > 0)
		{
			U1TXREG = txb.data[txb.head];
			txb.head = (++txb.head) & INDEX_MASK;
			txb.count--;
		}
		if(txb.count == 0)
		{
			DisableUart1TxInterrupt();
		}
		IFS0bits.U1TXIF = 0;
	}
}


