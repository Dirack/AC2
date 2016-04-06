#include <detpic32.h>

void configureUART(int, char, unsigned int);
void putc(char);

int main(void){
  EnableInterrupts();
  configureUART(115200, 'N', 1);

  while(1);
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
  
  // configurationof Interrupts
  IEC0bits.U1RXIE = 1;
  IPC6bits.U1IP = 3;
  
  U1MODEbits.ON = 1;          // enable UART1
}

void putc(char c){
  while (U1STAbits.UTXBF == 1);
  U1TXREG = c;
}

void _int_(24) isr_uart1(void){
  char c = ' ';

  if (IFS0bits.U1EIF == 1){           // If U1EIF set then
    if (U1STAbits.OERR == 1)          // If overrun error then clear OERR flag
      U1STAbits.OERR = 0;
    else                              // else read U1RXREG to a dummy variable
      c = U1RXREG;
    IFS0bits.U1RXIF = 0;              // clear UART1 error interrupt flag
  }

  putc(U1RXREG);
  IFS0bits.U1RXIF = 0;
}
