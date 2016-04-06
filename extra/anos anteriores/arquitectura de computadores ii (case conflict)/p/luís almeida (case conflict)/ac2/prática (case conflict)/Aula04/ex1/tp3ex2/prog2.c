#include <detpic32.h>

int main(void){
  
  // configuracao do bit RE0, RE1, RE2, RE3 como saida e RE6 e RE7 como entrada 0011 0000;
  TRISE &=0x00C0;

  while(1){
    LATEbits.LATE0 = PORTEbits.RE6 & PORTEbits.RE7;
    LATEbits.LATE1 = PORTEbits.RE6 | PORTEbits.RE7;
    LATEbits.LATE2 = PORTEbits.RE6 ^ PORTEbits.RE7;
    LATEbits.LATE3 = ~(PORTEbits.RE6 | PORTEbits.RE7);
  }
}
