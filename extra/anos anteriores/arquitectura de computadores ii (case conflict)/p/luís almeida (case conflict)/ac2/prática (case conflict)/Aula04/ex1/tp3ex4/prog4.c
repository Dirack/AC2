#include <detpic32.h>
#define CALIBRATION_VALUE 4998

// declaracao de funcao que se encontra definida apos o main e esta a ser invocada no main
void delay(unsigned int);

int main(void){
  while(1){
    resetCoreTimer();
    delay(1);
    printInt(readCoreTimer(), 10 + (10 << 16));
    printStr("\r\n");
  }
}

/*
####################################
# delay                            #
####################################

# para 6000 deu 24010 
# para 12000 deu 48010 
# a recta obtida foi y = m * x + b
# onde m = 4, b = 10 , logo como queremos y = 20000
# resolvendo vem x = 4997.5 -> arredonda-se para 4998 

*/

void delay(unsigned int n_intervalos){
  volatile unsigned int i;

  for(; n_intervalos != 0; n_intervalos--)
    for(i = CALIBRATION_VALUE; i != 0; i--)
      ;
}

