#include <detpic32.h>

#define DisableUart1Rx1Interrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1

#define BUF_SIZE = 8
#define INDEX_MASK (BUF_SIZE - 1)

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
	count++; //Increment "count" variable
	EnableUart1TxInterrupt(); //End of critical section
}

void comDrv_puts(char *s){
	int i = 0;
	while(s[i] != '\0'){
		comDrv_putc(s[i]);
		i++;
	}
}

void _int_(28) isr_uart1(void){
	if(U1TXbits.IFS0 == 1)// If U1TXIF is set
	{
		if(txb.count > 0)// If "count" variable (transmission buffer, txb) is greater than 0
		{
			U1TXREG = txb.head;// Copy character pointed by "head" to U1TXREG register
			txb.head = (txb.head + 1) & INDEX_MASK; // Increment "head" variable (mod BUF_SIZE)
			txb.count--; // Decrement "count" variable
		}
		if(txb.count == 0)// If "count" variable is 0 then
			DisableUart1TxInterrupt();
		U1TXbits.IFS0 = 0;// Reset UART1 TX interrupt flag
	}
}
