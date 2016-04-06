#include <detpic32.h>

#define DisavleUart1RxInterrupt() IEC0bits.U1RXIE = 0;
#define EnableUart1RxInterrupt()  IEC0bits.U1RXIE = 1;
#define DisavleUart1TxInterrupt() IEC0bits.U1TXIE = 0;
#define EnableUart1TxInterrupt()  IEC0bits.U1TXIE = 1;

int main(void){
  return 0;
}

typedef struct{
  unsigned char data[BUF_SIZE];
  unsigned int head;
  unsigned int tail;
  unsigned int count;
} circularBuffer;

volatile circularBuffer rxb;
volatile circularBuffer txb;
