#include <detpic32.h>
#define CALIBRATION_VALUE 4998

// declaracao de funcao delay e send2displays
void send2displays(unsigned char);
void delay(unsigned int);

int main(void){
  // configurar os portos RB0 a RB9 como saida
  TRISB=0xFC00;

  // definir todos os leds a zero
  PORTB = 0x0000;

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
  int i, a, tmedio, soma, media;
  
  a = 0;
  while(1){
    delay(10);
    if (a++ == 25){
      soma = 0;
      media = 0;
      tmedio = 0;
      AD1CON1bits.ASAM = 1;         // iniciar conversoes
      while(IFS1bits.AD1IF == 0);   // aguardar que a conversao termine. AD1IF a 1 indica que a conversao acabou
      for (i = 0; i < 4 ; i++){
        printInt(p[i*4], 0x0004000A);
        soma += p[i*4];             // para calculo da soma das 4 conversoes
        printStr(" ");
      }
      printStr("media = ");
      media = soma / 4;                 // calculo da media
      printInt(media, 0x0004000A);      // impressao da media
      printStr(" Vmed = ");
      tmedio = (media*33)/1023;         // calculo da tensao media
      printInt(tmedio, 0x0002000A);     // impressao da tensao
      printStr("\r");
      IFS1bits.AD1IF = 0;               // reset manual do bit AD1IF
      a = 0;
    }
    send2displays(tmedio);
  }
}

void send2displays(unsigned char value){
  static unsigned char displayFlag = 0;
  static const unsigned char display7codes [] = {
    0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E, 0x7E
  };

  int a, b;
  a = value % 10;
  b = (value - a)/10;

  if (displayFlag == 0){
    PORTB = 0x0200 + display7codes[a];
  }else{
    /* adicionei 0x80 para aparecer sempre o ponto quando é imprimido
    o valor do select high... Por alguma razao que não percebi, se
    fizesse um ciclo externo para imprimir o ponto fora desta funcao, o
    a tensao maxima de 3.3 nunca aparecia... dava sempre 3.2 e se 
    tentar neste momento meter a zero, o valor minimo que obtenho 
    é 0020 no terminal, nas no arredondamento dos displays aparece 0.0,
    neste caso nao é preocupante, mas se tivesse mais displays para mostrar
    as unidades centesimas e milesimas já teria problemas de novo... 
    nao percebo porque razao isto acontece...*/
    PORTB = 0x0100 + display7codes[b] + 0x80;
  }
  displayFlag = ~displayFlag;
}

void delay(unsigned int n_intervals){
  volatile unsigned int i;

  for (; n_intervals != 0; n_intervals--)
    for (i = CALIBRATION_VALUE; i != 0; i--)
      ;
}
