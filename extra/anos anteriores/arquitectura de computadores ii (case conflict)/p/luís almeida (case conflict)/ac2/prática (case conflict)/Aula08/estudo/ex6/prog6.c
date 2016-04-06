#include <detpic32.h>

#define freq 2

void configureUART(int, char, unsigned int);
void putc(char);
void sendString(char *);

int main(void){
  // configuration of RE0 as output
  TRISEbits.TRISE0 = 0;

  configureUART(115200, 'N', 1);

  while(1){
    while(U1STAbits.TRMT != 1);
    LATEbits.LATE0 = 1;
    sendString("123456789\n");
    LATEbits.LATE0 = 0;
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

void sendString(char *frase){
  char *p = frase;
  while( *p != '\0'){
    putc(*p);
    p++;
  }
}
