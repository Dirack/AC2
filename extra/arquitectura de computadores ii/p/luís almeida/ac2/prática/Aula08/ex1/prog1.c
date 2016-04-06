#include <detpic32.h>

void configureUART(int);
void putc(char);

int main(void){
  configureUART(115200);

  while(1){
    putc('+');
    resetCoreTimer();
    while(readCoreTimer() < 20000000);
  }
}

void configureUART(int baudrate){
  
  U1MODEbits.BRGH = 0;
  U1BRG = ((PBCLK + 8 * baudrate)/(16 * baudrate)) - 1;

  U1MODEbits.PDSEL = 0;                   // ver folha 7 da UART section
  U1MODEbits.STSEL = 0;                   // ver folha 7 da UART section
  U1STAbits.UTXEN = 1;
  U1STAbits.URXEN = 1;
  U1MODEbits.ON = 1;
}

void putc(char c){
  while(U1STAbits.UTXBF == 1);
  U1TXREG = c;
}
