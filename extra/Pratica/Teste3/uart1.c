#include <detpic32.h>

void putc(char byte2send){
	while(U1STAbits.UTXBF == 1);
	U1TXREG = byte2send;
}
void puts(char *s){
	int i = 0;
	while(s[i] != '\0'){
		putc(s[i]);
		i++;
	}
}
char getc(void){
	if(U1STAbits.OERR == 1) U1STAbits.OERR = 0;
	while(U1STAbits.URXDA == 0);
	return U1RXREG;
}
int main(void){
	
	U1MODEbits.BRGH = 0;
	U1BRG = 10;
	
	U1MODEbits.PDSEL = 0;
	U1MODEbits.STSEL = 0;
	
	IFS0bits.U1RXIF = 0;
	IEC0bits.U1RXIE = 1;
	IPC6bits.U1IP = 2;
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	U1MODEbits.ON = 1;
	
	EnableInterrupts();
	while(1){
		//putc(getc());
		getc();
	}

}

void _int_(24) isr_uart1(void){
	putc(U1RXREG);
	IFS0bits.U1RXIF = 0;
	
}
