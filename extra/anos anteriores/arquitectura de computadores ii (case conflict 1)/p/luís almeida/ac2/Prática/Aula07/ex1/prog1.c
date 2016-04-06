#include <detpic32.h>

void set_freq_23(int Hz);

int main(void){

  // configuracao de Timer
  //T3CONbits.TCKPS = 7;          // 1:32 prescaler (i.e. fin = 256Hz)
  //PR3 = PBCLK/256/2-1;          // PR3 = (20 MHz / 32 / 2) - 1)) =  Mhz
                                  // logo fout = fin / (PRx + 1)

  set_freq_23(2);
  TMR3 = 0;                       // Reset Timer T3 count register
  T3CONbits.TON = 1;              // Enable timer T3 (must be the last command
                                  // of the timer configuration sequence)

  while(1){
    while(IFS0bits.T3IF == 0);    //wait until T3IF = 1;
    IFS0bits.T3IF = 0;            // Reset T3IF
    putChar('.');
  }
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
