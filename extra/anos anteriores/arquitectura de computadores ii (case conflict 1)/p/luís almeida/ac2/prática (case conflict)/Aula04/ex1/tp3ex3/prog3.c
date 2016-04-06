#include <detpic32.h>
#define frequencia 5000000

int main(void){

  // configuracao dos bits RE0, RE1, RE2 e RE3 como saida e RE6, RE7 como entrada
  TRISE &=0x00C0;
  
  resetCoreTimer();
  int contador = 0;

  int numero = 0;
loop:;
  while(1){
    // verificacao demudanca de contador
    if (PORTEbits.RE6 != contador){
      contador = PORTEbits.RE6;
      LATE &=0x0000;
      numero = 0;
    }

    int time = readCoreTimer();

    // impressao do contador
    if (time < frequencia){
      goto loop;
    }else{
      if (PORTEbits.RE6 == 0){
        if (PORTEbits.RE7 == 0){
          numero += 1;
          if (numero == 16){
            LATE &=0x0000;
            numero = 0;
            resetCoreTimer();
            goto loop;
          }else{
            LATE = numero;
            resetCoreTimer();
            goto loop;
          }
        }else{
          numero -= 1;
          if (numero < 0){
            LATE &=0x000F;
            numero = 16;
            resetCoreTimer();
            goto loop;
          }else{
            LATE = numero;
            resetCoreTimer();
            goto loop;
          }
        }
      }else{
        if(PORTEbits.RE7 == 0){
          numero <<= 1;
          numero += 1;
          if (numero > 15){
            LATE &=0x0000;
            numero = 0;
            resetCoreTimer();
            goto loop;
          }else{
            LATE = numero;
            resetCoreTimer();
            goto loop;
          }
        }else{
          numero >>= 1;
          if (numero == 0){
            LATE = 0x0000;
            numero = 0x001F;
            resetCoreTimer();
            goto loop;
          }else{
            LATE = numero;
            resetCoreTimer();
            goto loop;
          }
        }
      }
    } 
  }
}
