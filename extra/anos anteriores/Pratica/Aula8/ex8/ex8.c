# include <detpic32.h>
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
	configUart(19200,'N',1);
	// config RE0 as output
	TRISEbits.TRISE0 = 0;
	
	while(1)
	{
		// Wait until TRMT == 1
		//while(U1STAbits.TRMT != 1);
		// Set RE0
		LATEbits.LATE0 = 1;
		puts("12345"); //440us 880us always 1 (115200,57600,19200)
		// Reset RE0
		LATEbits.LATE0 = 0;
	}
}
