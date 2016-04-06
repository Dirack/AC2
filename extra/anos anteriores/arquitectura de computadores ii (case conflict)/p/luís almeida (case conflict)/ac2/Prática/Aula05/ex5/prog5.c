#include <detpic32.h>

int main(void){

  //configuracao das entradas analogicas
  TRISBbits.TRISB14 = 1;    // Desliga saida digital de RB14
  AD1PCFGbits.PCFG14 = 0;   // Configura RB14 como entrada analogica

  //Seleccao do canal de entrada
  AD1CHSbits.CH0SA = 14;    // Escolhe o AN14 como entrada do conversor

  // configuracoes adicionais
  AD1CON1bits.SSRC = 7;     // conversion trigger selection bits: in this mode an internal counter ends sampling and starts conversion
  AD1CON1bits.CLRASAM = 1;   // Stop conversions when the first A/D converterinterruptis generated. At the same time, hardware clears the ASM bit
  AD1CON3bits.SAMC = 16;    // sample time is 16 TAD (TAD = 100 ns)
  AD1CON2bits.SMPI = 4-1;   // Interrupt is generated after XX samples replace XX by the desired number of consecutive samples (neste caso XX-1, onde XX = 4)

  AD1CON1bits.ON = 1;       // enable A/D converter. This must be the last comand of the A/D configuration sequence

  int *p = (int *)(&ADC1BUF0);
  int i;
  
  while(1){
    AD1CON1bits.ASAM = 1;         // iniciar conversoes
    while(IFS1bits.AD1IF == 0);   // aguardar que a conversao termine. AD1IF a 1 indica que a conversao acabou
    for (i = 0; i < 16 ; i++){
      printInt(p[i*4], 0x0004000A);
      printStr(" ");
    }
    printStr("\r");
    IFS1bits.AD1IF = 0;           // reset manual do bit AD1IF
  }

  return 0;
}
