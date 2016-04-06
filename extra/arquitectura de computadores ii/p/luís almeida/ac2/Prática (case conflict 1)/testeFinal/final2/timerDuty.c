#include <detpic32.h>

// declaracao de funcoes
void configureAll(void);
void set_freq_3(int);
void setPWM(int);
void binary(void);

//declaracao de variaveis
volatile int valueBinary = 0;

int main(void){
  configureAll();
  EnableInterrupts();

  while(1){
    if (PORTEbits.RE5 == 1 && PORTEbits.RE4 == 1){
      IEC0bits.T3IE = 1;
      setPWM(40);
    }else{
      IEC0bits.T3IE = 0;
      setPWM(100);
      LATE = 0;
    }
  }
}

void configureAll(void){
  TRISE = 0x00F0;

  set_freq_3(4);
  TMR3 = 0;
  T3CONbits.TON = 1;

  IFS0bits.T3IF = 0;
  IEC0bits.T3IE = 1;
  IPC3bits.T3IP = 1;
}

void set_freq_3(int Hz){
  static int shift [] = {0, 1, 2, 3, 4, 5, 6, 8};
  int i, c;

  c = (PBCLK + Hz/2)/Hz;
  for (i = 0; i < 8 && (c >> shift[i]) > 65536; i++);
  if (i == 8)
    exit(1);

  PR3 = (c >> shift[i]) - 1;
  T3CONbits.TCKPS = i;
}

void _int_(12) isr_T3(void){
  binary();
  IFS0bits.T3IF = 0;
}

void setPWM(int dutyCycle){
  if (dutyCycle < 0 || dutyCycle > 100)
    exit(1);

  OC1RS = dutyCycle * (PR3 + 1)/100;

  OC1CONbits.OCM = 6;
  OC1CONbits.OCTSEL = 1;
  OC1CONbits.ON = 1;
}

void binary(void){
  if (valueBinary > 15)
    valueBinary = 0;
  LATE = valueBinary;
  valueBinary++;
}
