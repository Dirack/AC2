#include <detpic32.h>

void configureUART(int);
void putc(char);
void sendString(char *);

int main(void){
  configureUART(115200);

  while(1){
    sendString("caramelo");
    resetCoreTimer();
    while(readCoreTimer() < 20000000);
  }
}

void configureUART(int baudrate){
  
  U1MODEbits.BRGH = 0;
  U1BRG = ((PBCLK + 8 * baudrate)/(16 * baudrate)) - 1;

  U1MODEbits.PDSEL = 0;         // ver folha 7 da UART section
  U1MODEbits.STSEL = 0;         // ver folha 7 da UART section
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
