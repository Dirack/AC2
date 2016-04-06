#include <detpic32.h>

#define freq 2
#define stringTeste "caramelo"

void configureUART(int);
void configureTimer(void);
void putc(char);
void set_freq_2(int);
void sendString(char *);

volatile int timerFlag = 0;

int main(void){
  configureUART(115200);
  configureTimer();
  EnableInterrupts();

  while(1);
}

void configureTimer(void){
  set_freq_2(freq);
  TMR2 = 0;
  T2CONbits.TON = 1;

  IEC0bits.T2IE = 1;
  IPC2bits.T2IP = 1;
}

void configureUART(int baudrate){
  U1MODEbits.BRGH = 0;
  U1BRG = ((PBCLK + 8 * baudrate)/(16 * baudrate)) - 1;

  U1MODEbits.PDSEL = 0;       // ver folha 7 da UART section
  U1MODEbits.STSEL = 0;       // ver folha 7 da UART section
  U1STAbits.UTXEN = 1;        // enable transmiter
  U1STAbits.URXEN = 1;        // enable receiver
  U1MODEbits.ON = 1;          // enable UART1
}

void putc(char c){
  while (U1STAbits.UTXBF == 1);
  U1TXREG = c;
}

void _int_(8) isr_T2(void){
  timerFlag++;

  if (timerFlag == 2){
    sendString(stringTeste);
    timerFlag = 0;
  }
  
  IFS0bits.T2IF = 0;
}

void sendString(char *frase){
  char *p = frase;
  while( *p != '\0'){
    putc(*p);
    p++;
  }
}

void set_freq_2(int Hz){
  static int shift [] = {0, 1, 2, 3, 4, 5, 6, 8};
  int i, c;

  c = (PBCLK + Hz/2)/Hz;
  for (i = 0; i < 8 && (c >> shift[i]) > 65536; i++);
  if (i == 0)
    exit(1);

  PR2 = (c >> shift[i]) - 1;
  T2CONbits.TCKPS = i;
}
