#include <detpic32.h>
#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0;
#define EnableUart1RxInterrupt()  IEC0bits.U1RXIE = 1;
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0;
#define EnableUart1TxInterrupt()  IEC0bits.U1TXIE = 1;

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)

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

void comDrv_config(unsigned int baud, char parity, unsigned int stopbits){
	U1MODEbits.BRGH = 0;
	if(baud >= 600 && baud <= 115200)
		U1BRG = ((20000000+8*baud)/(16*baud))-1;
	else U1BRG = 10;

	if(parity == 'O' || parity == 'o'){
		U1MODEbits.PDSEL = 2;
	}
	else if(parity == 'E' || parity == 'e'){
		U1MODEbits.PDSEL = 1;
	}
	else U1MODEbits.PDSEL = 0;

	if(stopbits == 2){
		U1MODEbits.STSEL = 1;	
	}
	else U1MODEbits.STSEL = 0;

	U1STAbits.URXEN = 1; //transmission enabled
	U1STAbits.UTXEN = 1; //reception enabled
	U1MODEbits.ON = 1; //UART enabled 

	//Interruptions
	IEC0bits.U1RXIE = 0;
	IFS0bits.U1RXIF = 0;
	IEC0bits.U1TXIE = 0;
	IFS0bits.U1TXIF = 0;
	IPC6bits.U1IP = 2;

}

int main(void){
	comDrv_config(115200,'N',1);
	comDrv_flushRx();
	comDrv_flushTx();
	EnableInterrupts();
	while(1)
		comDrv_puts("Teste de transmissao\n ");
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
}
