#include <detpic32.h>

// declaracao de funcoes
void set_freq_3(int Hz);

int main(void){

  //configuração do Timer3
  set_freq_3(100);
  TMR3 = 0;
  T3CONbits.TON = 1;

  // configuracao de duty-cycle
  OC1CONbits.OCM = 6;         // PWM mode on OC1
  OC1CONbits.OCTSEL = 1;      // select timer3
  OC1RS = 12500;              // pwm on constant
  OC1CONbits.ON = 1;          // enable OC1

  while(1);
  return 0;
}

void set_freq_3(int Hz){
  static int shift [8] = {0, 1, 2, 3, 4, 5, 6, 8};
  int i, c;

  c = (PBCLK + Hz/2)/Hz;
  for (i = 0; i < 8 && (c >> shift[i]) > 65536; i++);
  if (i == 0)
    exit(1);
  PR3 = (c >> shift[i]) - 1;
  T3CONbits.TCKPS = i;
}
