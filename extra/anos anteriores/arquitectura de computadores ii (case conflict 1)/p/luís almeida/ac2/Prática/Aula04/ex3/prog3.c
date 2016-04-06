#include <detpic32.h>

int main(void){

  // configurar os portos RB0 a RB9 como saida
  TRISB =0xFC00;

  // definir todos os leds a zero
  LATB =0x0000;

  while(1){
    char a = getChar();
    putChar(a);
    if (a == 'a' || a == 'A'){
      LATB =0x015F;
    }else if (a == 'b' || a == 'B'){
      LATB =0x017C;
    }else if (a == 'c' || a == 'C'){
      LATB =0x0136;
    }else if (a == 'd' || a == 'D'){
      LATB =0x0179;
    }else if (a == 'e' || a == 'E'){
      LATB =0x013E;
    }else if (a == 'f' || a == 'F'){
      LATB =0x011E;
    }else if (a == 'g' || a == 'G'){
      LATB =0x017E;
    }else if (a == '.'){
      LATB =0x0180;
    }else{
      LATB =0x0000;
    }
  }
}
