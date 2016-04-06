#include <detpic32.h>
#define CALIBRATION_VALUE 4998

// declaracao de funcao delay
void delay(unsigned int);

int main(void){
  static const unsigned char codes [] = {0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E, 0x7E};

  // configurar os portos RB0 a RB9 como saida
  TRISB=0xFC00;

  // definir todos os leds a zero
  LATB=0x0000;
  
  int i;

  while(1){
    // selecao de display low
    int low = 0x0200;
    for (i = 0; i < 7; i++){
       PORTB = low + codes[i];
       delay(1000);
    }
    
    // selecao de display high
    int high = 0x0100;
    for (i = 0; i < 7; i++){
       PORTB = high + codes[i];
       delay(1000);
    }
  }
}

void delay(unsigned int n_intervalos){
  volatile unsigned int i;

  for (; n_intervalos != 0; n_intervalos--)
    for(i = CALIBRATION_VALUE; i != 0; i--)
      ;
}
