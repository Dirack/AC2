#include <detpic32.h>

void set_freq_23(int Hz);

int main(void){

  // configuracao de Timer
  //T3CONbits.TCKPS = 7;          // 1:32 prescaler (i.e. fin = 256Hz)
  //PR3 = PBCLK/256/2-1;          // PR3 = (20 MHz / 32 / 2) - 1)) =  Mhz
  set_freq_23(2);
  TMR3 = 0;                       // Reset Timer T3 count register
  T3CONbits.TON = 1;              // Enable timer T3 (must be the last command
                                  // of the timer configuration sequence)

  // configuracao de interrupcoes
  IFS0bits.T3IF = 0;              // Reset timer T3 interrrupt Flag
  IPC3bits.T3IP = 2;              // Interrupt priority (must be in range of [1..6])
  IEC0bits.T3IE = 1;              // enable timer 3 interrupts
  
  EnableInterrupts();
  while(1);
}

void _int_(12) isr_T3(void){  // replace _int_(VECTOR) for the T3 timer vector number
  putChar('.');
  IFS0bits.T3IF = 0;
}

void set_freq_23(int Hz){
  static int shift [8]={0, 1, 2, 3, 4, 5, 6, 8}; // [1, 2, 4, 8, 16, 32, 64, 256]
  int i, c;

  c = (PBCLK + Hz/2)/Hz;
  for ( i = 0; i < 8 && (c >> shift[i]) > 65536; i++)
    ;
  if (i == 8)
    exit(1);
  PR3 = (c >> shift[i])-1;
  T3CONbits.TCKPS = i;
}
