#include <detpic32.h>

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0;
#define EnableUart1RxInterrupt()  IEC0bits.U1RXIE = 1;
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0;
#define EnableUart1TxInterrupt()  IEC0bits.U1TXIE = 1;
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)

void comDrv_flushRx(void);
void comDrv_flushTx(void);
void comDrv_putc(char);
void comDrv_put_s(char *);
void comDrv_config(unsigned int, char, unsigned int);

int main(void){
  comDrv_config(115200, 'N', 1);
  comDrv_flushRx();
  comDrv_flushTx();
  EnableInterrupts();

  while(1){
    comDrv_put_s("abcdefg\n");
  }
}

typedef struct{
  unsigned char data[BUF_SIZE];
  unsigned int head;
  unsigned int tail;
  unsigned int count;
} circularBuffer;

volatile circularBuffer rxb;
volatile circularBuffer txb;

void comDrv_flushRx(void){
  int i = 0;
  for (i = 0; i < BUF_SIZE; i++){
    rxb.data[i] = '\0';
  }
  rxb.head = 0;
  rxb.tail = 0;
  rxb.count = 0;
}

void comDrv_flushTx(void){
  int i = 0;
  for (i = 0; i < BUF_SIZE; i++){
    txb.data[i] = '\0';
  }
  txb.head = 0;
  txb.tail = 0;
  txb.count = 0;
}

void comDrv_putc(char c){
  while(txb.count == BUF_SIZE);
  txb.data[txb.tail] = c;
  txb.tail = (txb.tail + 1) & INDEX_MASK;
  DisableUart1TxInterrupt();
  txb.count++;
  EnableUart1TxInterrupt();
}

void comDrv_put_s(char *c){
  int i = 0;
  for (i = 0; i < c[i] != '\0'; i++){
    comDrv_putc(c[i]);
  }
  comDrv_putc(c[i]);
}

void _int_(24) U1XXIF_Interrupts(void){
  if (IFS0bits.U1TXIF == 1){
    if (txb.count > 0){
      U1TXREG = txb.data[txb.head];
      txb.head = (txb.head + 1) & INDEX_MASK;
      txb.count--;
    }

    if (txb.count == 0){
      DisableUart1TxInterrupt();
    }
    IFS0bits.U1TXIF = 0;
  }
}

void comDrv_config(unsigned int baudrate, char parity, unsigned int stopBits){
  // validacao de valores aceitaveis
  if (baudrate < 600 || baudrate > 115200)
    baudrate = 115200;
  if (parity != 'E' || parity != 'N' || parity != 'O')
    parity = 'N';
  if (stopBits != 1 || stopBits != 2)
    stopBits = 1;

  while(U1STAbits.TRMT == 0);     // espera até o buffer ficar vazio

  U1MODEbits.BRGH = 0;            // 16 bit division factor
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
    U1MODEbits.STSEL = 1;         // ver folha 7 da UART section
  }

  U1STAbits.UTXEN = 1;          // enable transmiter
  U1STAbits.URXEN = 1;          // enable receiver
  
  U1STAbits.UTXSEL = 0;
  IFS0bits.U1TXIF = 0;
  IPC6bits.U1IP = 2;

  U1MODEbits.ON = 1;            // enable UART1
}
