#include <detpic32.h>
#define CALIBRATION_VALUE 4998

// declaracao de funcao delay e send2displays
void send2displays(unsigned char);
void delay(unsigned int);

int main(void){
  // configurar os portos RB0 a RB9 como saida
  TRISB=0xFC00;

  // definir todos os leds a zero
  LATB=0x0000;
  
  char valor = 0b00000000;
  int i, a;

  while(1){
    i = 0;
    do{
      delay(5);
      send2displays(valor);
      delay(5);
      if (valor % 2 == 0){
        PORTB = 0x0280;
      }else{
        PORTB = 0x0180;
      }
    }while(++i < 100);

    valor += 0b00000001;
    
    if (valor == 0b01100100){
      valor = 0b00000000;
      i = 0;

      do{
        PORTB = 0x0000;
        delay(500);

        a = 0;
        do{
          delay(5);
          send2displays(valor);
          delay(5);
          PORTB = 0x0280;
        }while(++a < 50);
      }while(++i < 5);
      valor += 0b00000001;
    }
  }
}

void send2displays(unsigned char value){
  static unsigned char displayFlag = 0;
  static const unsigned char display7Scodes [] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E, 0x7E};

  int a, b;
  a = value % 10;
  b = (value - a)/10;

  if (displayFlag == 0){
    PORTB = 0x0200 + display7Scodes[a];
  }else{
    PORTB = 0x0100 + display7Scodes[b];
  }
  displayFlag = ~displayFlag;
}

void delay(unsigned int n_intervals){
  volatile unsigned int i;

  for (; n_intervals != 0; n_intervals--)
    for (i = CALIBRATION_VALUE; i != 0; i--)
      ;
}
