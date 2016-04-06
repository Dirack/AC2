#include <detpic32.h>

int main(void){
  static const unsigned char codes [] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E, 0x7E};

  // configurar os portos RB0 a RB9 como saida
  TRISB=0xFC00;
  // configurar os portos RE4 a RE7 como entrada
  TRISEbits.TRISE4 = 1;
  TRISEbits.TRISE5 = 1;
  TRISEbits.TRISE6 = 1;
  TRISEbits.TRISE7 = 1;

  // definir todos os leds a zero
  LATB=0x0000;
  
  int valor;

  while(1){
    valor = PORTEbits.RE4*1 + PORTEbits.RE5*2 + PORTEbits.RE6*4 + PORTEbits.RE7*8;
    // apresentacao nos dois selects ao mesmo tempo
    LATB = 0x0300 + codes[valor];
  }
}
