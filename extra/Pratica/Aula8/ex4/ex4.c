#include <detpic32.h>

void configUart(unsigned int baud, char parity, unsigned int stopbits)
{
	// Configure BaudRate Generator
	if(baud >= 600 && baud <= 115200){
		U1MODEbits.BRGH = 0;
		U1BRG = (20000000+(baud*8))/(baud*16);
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
