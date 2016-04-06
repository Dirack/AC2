#include <detpic32.h>

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)

#define TRUE 1;
#define FALSE 0;
void comDrv_config(unsigned int baud, char parity, unsigned int stopbits)
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
	// UART1 Interruptions
	U1STAbits.UTXSEL = 0;
	U1STAbits.URXISEL = 0;
	
	IFS0bits.U1RXIF = 0;
	IEC0bits.U1RXIE = 1;
	
	IFS0bits.U1TXIF = 0;
	IEC0bits.U1TXIE = 0;
	
	IPC6bits.U1IP = 2;

	// Enable the trasmitter and receiver modules
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;

	// Enable UART1
	U1MODEbits.ON;
}

typedef struct
{
	unsigned char data[BUF_SIZE];
	unsigned int head;
	unsigned int tail;
	unsigned int count;
} circularBuffer;

volatile circularBuffer txb; //Transmission buffer
volatile circularBuffer rxb; //Reception buffer

void comDrv_flushRx(void){
	rxb.head = rxb.tail = rxb.count = 0;
}

void comDrv_flushTx(void){
	txb.head = txb.tail = txb.count = 0;
}

void comDrv_putc(char ch){
	while(txb.count == BUF_SIZE); //Wait while buffer is full
	txb.data[txb.tail] = ch; //Copy character to the transmission buffer at position "tail"
	txb.tail = (txb.tail + 1) & INDEX_MASK; // Incremenent "tail" index (mod.BUF_SIZE)
	DisableUart1TxInterrupt(); //Begin of critical section
	txb.count++; //Increment "count" variable
	EnableUart1TxInterrupt(); //End of critical section
}

void comDrv_puts(char *s){
	int i = 0;
	while(s[i] != '\0'){
		comDrv_putc(s[i]);
		i++;
	}
}

char comDrv_getc(char *pchar)
{
	if(rxb.count == 0) return FALSE;// Test "count" variable (reception buffer) and return FALSE if it is zero
	DisableUart1RxInterrupt();
	// Begin of critical section
	*pchar = rxb.data[rxb.head]; // Copy character pointed by "head" to *pchar
	rxb.count--;// Decrement "count" variable
	rxb.head = (rxb.head + 1) & INDEX_MASK;// Increment "head" variable (mod BUF_SIZE)
	EnableUart1RxInterrupt();
	//End of critical section
	return TRUE;
}

void _int_(24) isr_uart1(void){
	if(IFS0bits.U1TXIF == 1)// If U1TXIF is set
	{
		if(txb.count > 0)// If "count" variable (transmission buffer, txb) is greater than 0
		{
			U1TXREG = txb.data[txb.head];// Copy character pointed by "head" to U1TXREG register
			txb.head = (txb.head + 1) & INDEX_MASK; // Increment "head" variable (mod BUF_SIZE)
			txb.count--; // Decrement "count" variable
		}
		if(txb.count == 0)// If "count" variable is 0 then
			DisableUart1TxInterrupt();
		IFS0bits.U1TXIF = 0;// Reset UART1 TX interrupt flag
	}

	if(IFS0bits.U1RXIF == 1) //If U1RX1F is set
	{
		rxb.data[rxb.tail] = U1RXREG; // Read character from UART and write it to the tail position of the reception buffer
		rxb.tail = (rxb.tail + 1) & INDEX_MASK; //Increment "tail" variable (mod BUF_SIZE)
		if(rxb.count < BUF_SIZE) 	
			rxb.count++; //If reception buffer is not full increment count variable
		else rxb.head = (rxb.head + 1) & INDEX_MASK;//increment "head" variable (discard oldest character)
		IFS0bits.U1RXIF = 0; // reset UART1RX interrupt flag;
	}
}

void main(void)
{
	comDrv_config(115200,'N',1); // default "pterm" parameters
	// with RX interrupts enabled and TX
	// interrupts disabled
	// (...)
	EnableInterrupts();
	char c;
	char s[31];
	int strlen = 0;
	comDrv_puts("PIC32 UART Device-driver\n");
	while(1)
	{
		// Read character from reception buffer
		while(comDrv_getc(&c) == 0);
		// Send character to the transmission buffer
		if(c == 'S'){
			while(strlen <= 30 && s[strlen-1] != '\n'){
				while(comDrv_getc(&c) == 0);
				s[strlen] = c;
				strlen++;
			}
			s[strlen] = '\0';
			strlen = 0;
			comDrv_puts(s);
		}
		else comDrv_putc(c);
	}	
}
