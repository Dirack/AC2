#include <detpic32.h>
char getC(void){
	char trash;
	if(U1STAbits.OERR == 1) U1STAbits.OERR = 0;
	while(U1STAbits.URXDA == 0);
	if(U1STAbits.FERR == 1 || U1STAbits.PERR == 1){
		trash = U1RXREG;
		return 0;
	}
	return U1RXREG;
}
void putc(char byte2send){
	while(U1STAbits.UTXBF == 1);
	U1TXREG = byte2send;
}

void puts(char *str){
	int i = 0;
	while( str[i] != '\0'){
		putc(str[i]);
		i++;
	}
}

void configUart(unsigned int baud, char parity, unsigned int stopbits){
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
	IEC0bits.U1RXIE = 1;
	IFS0bits.U1RXIF = 0;

	IPC6bits.U1IP = 2;

}

int main(void)
{
	configUart(115200,'N',1);
	EnableInterrupts();
	
	while(1);	
}

void _int_(24) isr_uart1(void){
	putc(U1RXREG);
	IFS0bits.U1RXIF = 0;
}
