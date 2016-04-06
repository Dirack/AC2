#include <detpic32.h>

void configureUART(int, char, int);
void putc(char);
void sendString(char *);

int main(void){
  configureUART(115200, 'N', 1);

  while(1){
    sendString("caramelo");
    resetCoreTimer();
    while(readCoreTimer() < 20000000);
  }
}

void configureUART(int baudrate, char parity, int stopBits){
  // validacao de valores aceitaveis
  if (baudrate < 600 || baudrate > 115200)
    baudrate = 115200;
  if (parity != 'E' || parity != 'N' || parity != 'O')
    parity = 'N';
  if (stopBits != 1 || stopBits != 2)
    stopBits = 1;

  U1MODEbits.BRGH = 0;          // 16 bit division factor
  U1BRG = ((PBCLK + 8 * baudrate)/(16 * baudrate)) - 1;
  
  if (parity == 'O'){
    U1MODEbits.PDSEL = 2;         // ODD PARITY ver folha 7 da UART section
  }else if (parity == 'E'){
    U1MODEbits.PDSEL = 1;         // EVEN PARITY ver folha 7 da UART section
  }else{
    U1MODEbits.PDSEL = 0;         // NO PARITY ver folha 7 da UART section 
  }

  if (stopBits == 1){
    U1MODEbits.STSEL = 0;         // ver folha 7 da UART section
  }else{
    U1MODEbits.STSEL = 2;         // ver folha 7 da UART section
  }

  U1STAbits.UTXEN = 1;          // enable transmiter
  U1STAbits.URXEN = 1;          // enable receiver
  U1MODEbits.ON = 1;            // enable UART1
}

void sendString(char *frase){
  char *p = frase;
  while(*p != '\0'){
    putc(*p);
    p++;
  }
}

void putc(char c){
  while(U1STAbits.UTXBF == 1);
  U1TXREG = c;
}
