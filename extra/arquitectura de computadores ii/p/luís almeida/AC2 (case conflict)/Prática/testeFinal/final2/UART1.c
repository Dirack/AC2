#include <detpic32.h>

// declaracao de funcoes
void configureUART(int, char, int);
void putc(char);
char getc(void);

int main(void){
  configureUART(115200, 'N', 1);

  while(1){
    putc(getc());
  }
}

void configureUART(int baudrate, char parity, int stopBits){
  if (baudrate < 600 || baudrate > 115200)
    baudrate = 115200;

  U1BRG = (PBCLK + 8 * baudrate)/(16 * baudrate) - 1;
  U1MODEbits.BRGH = 0;

  if (parity == 'O' || parity == 'o'){
    U1MODEbits.PDSEL = 2;
  }else if(parity == 'E' || parity == 'e'){
    U1MODEbits.PDSEL = 1;
  }else{
    U1MODEbits.PDSEL = 0;
  }

  if (stopBits == 2){
    U1MODEbits.STSEL = 1;
  }else{
    U1MODEbits.STSEL = 0;
  }

  U1STAbits.URXEN = 1;
  U1STAbits.UTXEN = 1;

  U1MODEbits.ON = 1;
}

void putc(char byte2send){
  while(U1STAbits.UTXBF == 1);
  U1TXREG = byte2send;
}

char getc(void){
  while(U1STAbits.URXDA == 0);
  return U1RXREG;
}
