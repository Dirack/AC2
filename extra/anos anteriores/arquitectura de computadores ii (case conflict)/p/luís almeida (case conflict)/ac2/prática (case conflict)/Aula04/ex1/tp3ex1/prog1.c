#include <detpic32.h>

int main(void){

  // configuração do bit RE0 como bit de saida
  TRISEbits.TRISE0 = 0;

  // configuracao de bit RE6 como bit de entrada
  TRISEbits.TRISE6 = 1;

  while(1){
    int a = PORTEbits.RE6;
    if (a == 1){
      LATEbits.LATE0 = 0;
    }else{
      LATEbits.LATE0 = 1;
    }
  }
}
