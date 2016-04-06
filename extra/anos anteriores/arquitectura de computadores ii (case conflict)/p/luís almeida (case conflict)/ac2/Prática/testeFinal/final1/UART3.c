#include <detpic32.h>

#define enableTxInterrupts()   IEC0bits.U1TXIE = 1;
#define disableTxInterrupts()  IEC0bits.U1TXIE = 0;
#define enableRxInterrupts()   IEC0bits.U1RXIE = 1;
#define disableRxInterrupts()  IEC0bits.U1RXIE = 0;

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE -1)

void configureUART(int, char, int);
void rxbFlush(void);
void txbFlush(void);
void putc (char);
char getc (char *);
void put_s(char *);

int main(void){
  configureUART(115200, 'N', 1);
  EnableInterrupts();

  char p;
  while (1){
    while(getc(&p) == 0);
    putc(p);
  }
}

typedef struct{
  char data [BUF_SIZE];
  int head;
  int tail;
  int count;
}caramelo;

volatile caramelo rxb;
volatile caramelo txb;

void rxbFlush(void){
  int i;
  for (i = 0; i < BUF_SIZE; i++)
    rxb.data[i] = '\0';
  rxb.head = 0;
  rxb.tail = 0;
  rxb.count = 0;
}

void txbFlush(void){
  int i;
  for (i = 0; i < BUF_SIZE; i++)
    txb.data[i] = '\0';
  txb.head = 0;
  txb.tail = 0;
  txb.count = 0;
}

void configureUART(int baudrate, char parity, int stopBits){
  if (baudrate < 600 || baudrate > 115200)
    baudrate = 115200;

  U1BRG = (PBCLK + 8 * baudrate)/(16 * baudrate) - 1;
  U1MODEbits.BRGH = 0;

  if (parity == 'O' || parity == 'o'){
    U1MODEbits.PDSEL = 2;
  }else if (parity == 'E' || parity == 'e'){
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

  U1STAbits.UTXSEL = 1;
  IFS0bits.U1RXIF = 0;
  IFS0bits.U1TXIF = 0;
  IPC6bits.U1IP = 1;
  
  enableRxInterrupts();
  U1MODEbits.ON = 1;
}

void putc(char byte2send){
  while(txb.count == BUF_SIZE);
  txb.data[txb.tail] = byte2send;
  txb.tail = (txb.tail + 1)&INDEX_MASK;
  disableTxInterrupts();
  txb.count++;
  enableTxInterrupts();
}

void put_s(char *s){
  int i;
  for (i = 0; s[i] != '\0'; i++){
    putc(s[i]);
  }
  putc(s[i]);
}

char getc(char *p){
  if(rxb.count == 0){
    return 0;
  }else{
    disableRxInterrupts();
    *p = rxb.data[rxb.head];
    rxb.head = (rxb.head + 1)&INDEX_MASK;
    rxb.count--;  
    enableRxInterrupts();
  }
  return 1;
}

void _int_(24) isr_UART(void){
  if (IFS0bits.U1RXIF == 1){
    rxb.data[rxb.tail] = U1RXREG;
    rxb.tail = (rxb.tail + 1)&INDEX_MASK;
    
    if (rxb.count == BUF_SIZE){
      rxb.head = (rxb.head + 1)&INDEX_MASK;
    }else
      rxb.count++;

    IFS0bits.U1RXIF = 0;
  }
  if (IFS0bits.U1TXIF == 1){
    if (txb.count > 0){
      U1TXREG = txb.data[txb.head];
      txb.head = (txb.head + 1)&INDEX_MASK;
      txb.count--;
    }
    
    if (txb.count == 0)
      disableTxInterrupts();

    IFS0bits.U1TXIF = 0;
  }
}
