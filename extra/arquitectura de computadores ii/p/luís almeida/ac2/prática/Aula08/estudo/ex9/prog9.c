#include <detpic32.h>

void configureUART(int, char, unsigned int);
void putc(char);
char getc(void);

int main(void){
  configureUART(115200, 'N', 1);

  while(1){
    putc(getc());
  }
}

void configureUART(int baudrate, char parity, unsigned int stopBits){
  // validacao de valores aceitaveis
  if (baudrate < 600 || baudrate > 115200){
    baudrate = 115200;
  }

  if (parity != 'E' && parity != 'N' && parity != 'O'){
    parity = 'N';
  }

  if (stopBits != 1 && stopBits != 2){
    stopBits = 1;
  }

  U1MODEbits.BRGH = 0;        // 16 bit division factor
  U1BRG = ((PBCLK + 8 * baudrate)/(16 * baudrate)) - 1;
  
  // parity configuration
  if (parity == 'O'){
    U1MODEbits.PDSEL = 2;     // ODD parity ver folha 7 da UART section
  }else if (parity == 'E'){
    U1MODEbits.PDSEL = 1;     // EVEN parity ver folha 7 da UART section
  }else{
    U1MODEbits.PDSEL = 0;     // NO parity ver folha 7 da UART section
  }
  
  // stopBits configuration
  if (stopBits == 1){
    U1MODEbits.STSEL = 0;     // ver folha 7 da UART section
  }else{
    U1MODEbits.STSEL = 1;     // ver folha 7 da UART section
  }

  U1STAbits.UTXEN = 1;        // enable transmiter
  U1STAbits.URXEN = 1;        // enable receiver
  U1MODEbits.ON = 1;          // enable UART1
}

void putc(char c){
  while (U1STAbits.UTXBF == 1);
  U1TXREG = c;
}

char getc(void){
  // reset of OERR
  if (U1STAbits.OERR == 1){
    U1STAbits.OERR = 0;
  }
  
  while(U1STAbits.URXDA == 0);    // wait while URDXA == 0
    if (U1STAbits.FERR == 1 || U1STAbits.PERR == 1){    // If FERR == 1 or PERR == 1 then read UxRXREG and return 0;
      return 0;
    }
  return U1RXREG;                 // return U1RXREG
}
