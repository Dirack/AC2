#include <detpic32.h>
#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0;
#define EnableUart1RxInterrupt()  IEC0bits.U1RXIE = 1;
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0;
#define EnableUart1TxInterrupt()  IEC0bits.U1TXIE = 1;

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)
#define FALSE 0
#define TRUE 1

typedef struct{
	unsigned char data[BUF_SIZE];
	unsigned int head,tail,count;
} circularBuffer;

volatile circularBuffer txb; //transmission buffer
volatile circularBuffer rxb; //reception buffer

void comDrv_flushRx(){
	rxb.head = rxb.tail = rxb.count = 0;
}

void comDrv_flushTx(){
	txb.head = txb.tail = txb.count = 0;
}

void comDrv_putc(char ch){
	while(txb.count == BUF_SIZE); //wait while buffer is full
	txb.data[txb.tail] = ch; // copy char to the tail position
	txb.tail = (txb.tail + 1) & INDEX_MASK;
	DisableUart1TxInterrupt();
	txb.count++;
	EnableUart1TxInterrupt();
}

void comDrv_puts(char *s){
	int i = 0;
	while(s[i] != '\0'){
		comDrv_putc(s[i]);
		i++;
	}
}

char comDrv_getc(char *pchar){
	if(rxb.count == 0) return FALSE;
	DisableUart1RxInterrupt();
	*pchar = rxb.data[rxb.head];
	rxb.count--;
	rxb.head = (rxb.head + 1) & INDEX_MASK;
	EnableUart1RxInterrupt();
	return TRUE;
}

void comDrv_config(unsigned int baud, char parity, unsigned int stopbits){
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
	//Interruptions
	U1STAbits.UTXSEL = 0;
	U1STAbits.URXISEL = 0;
	
	IEC0bits.U1RXIE = 1;
	IFS0bits.U1RXIF = 0;
	IEC0bits.U1TXIE = 0;
	IFS0bits.U1TXIF = 0;
	IPC6bits.U1IP = 2;

	U1STAbits.URXEN = 1; //transmission enabled
	U1STAbits.UTXEN = 1; //reception enabled
	U1MODEbits.ON = 1; //UART enabled 

}

int main(void){
	comDrv_config(115200,'N',1);
	comDrv_flushRx();
	comDrv_flushTx();

	EnableInterrupts();
	comDrv_puts("PIC32 UART Device-driver\n");
	char c;
	char s[31];
	int i = 0;
	while(1){
		while(comDrv_getc(&c) == 0);
		if(c != 'S'){
			comDrv_putc(c);
		}
		else{
			while(i < 30 && s[i-1] != '\n'){
				while(comDrv_getc(&c) == 0);
				s[i] = c;
				i++;
			}
			s[i] = '\0';
			i = 0;	
			comDrv_puts(s);
		}	
	}
}

void _int_(24) isr_uart1(void){
	if(IFS0bits.U1TXIF == 1){
		if(txb.count > 0){
			U1TXREG = txb.data[txb.head];
			txb.head = (txb.head + 1) & INDEX_MASK;
			txb.count--;
		}
		if(txb.count == 0){
			DisableUart1TxInterrupt();
		}
		IFS0bits.U1TXIF = 0;
	}

	if(IFS0bits.U1RXIF == 1){
		rxb.data[rxb.tail] = U1RXREG;
		rxb.tail = (rxb.tail + 1) & INDEX_MASK;
		if( rxb.count < BUF_SIZE){
			rxb.count++;
		}
		else rxb.head = (rxb.head + 1) & INDEX_MASK;
		IFS0bits.U1RXIF = 0;
	}
}
