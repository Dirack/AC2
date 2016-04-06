#include <detpic32.h>
#define CALIBRATION_VALUE 4998

//declaracao de funcao delay e send2displays
void send2displays(unsigned char);
void delay(unsigned int);
volatile unsigned char tmedio = 0;    // Global variable

int main(void){
  // configurar os portos RB0 a RB9 como saida
  TRISB = 0xFC00;

  // definir todos os leds a zero
  PORTB = 0x0000;

  //configuracao das entradas analogicas
  TRISBbits.TRISB14 = 1;    // Desliga saida digital de RB14
  AD1PCFGbits.PCFG14 = 0;   // Configura RB14 como entrada analogica

  //Seleccao do canal de entrada
  AD1CHSbits.CH0SA = 14;    // Escolhe o AN14 como entrada do conversor

  // configuracoes adicionais
  AD1CON1bits.SSRC = 7;     // conversion trigger selection bits: in this mode an internal counter ends sampling and starts conversion
  AD1CON1bits.CLRASAM = 1;  // Stop conversions when the first A/D converterinterruptis generated. At the same time, hardware clears the ASM bit
  AD1CON3bits.SAMC = 16;    // sample time is 16 TAD (TAD = 100 ns)
  AD1CON2bits.SMPI = 8-1;   // Interrupt is generated after XX samples replace XX by the desired number of consecutive samples (neste caso XX-1, onde XX = 8)

  AD1CON1bits.ON = 1;       // enable A/D converter. This must be the last comand of the A/D configuration sequence

  // Interrupts Configuration
  IEC1bits.AD1IE = 1;
  IPC6bits.AD1IP = 3;       // por exemplo, pois como temos apenas uma
                            // coisa a correr nao interessa o valor, tem e
                            // de ser entre 1 a 6.

  IFS1bits.AD1IF = 0;             // Reset AD1IF flag
  EnableInterrupts();             // Global Interrupt Enable

  int i = 0;
  while(1){
    LATEbits.LATE0 = 0;           // Reset RE0
    
    delay(10);
    if (i++ == 25){
      AD1CON1bits.ASAM = 1;       // iniciar conversoes
      i = 0;
    }
    send2displays(tmedio);
  }
}

// Interrupt Handler
void _int_(27) isr_adc(void){
/* inside of int(XXXX) Replace vector by the A/D vector number - see
"PIC32 family dat sheet" (pages 122-124)*/
  int *p = (int *)(&ADC1BUF0);
  int a, soma = 0, media = 0;
    
  for (a = 0; a < 8 ; a++){
    printInt(p[a*4], 0x0004000A);
    soma += p[a*4];
    printStr(" ");
  }
  printStr("media = ");
  media = soma/8;
  printInt(media, 0x0004000A);
  printStr(" Vmed = ");
  tmedio = (media*33)/1023;
  printInt(tmedio, 0x0002000A);
  printStr("\r");

  // start A/D conversion
  IFS1bits.AD1IF = 0;             // Reset AD1IF flag
}

void send2displays(unsigned char value){
  static unsigned char displayFlag = 0;
  static const unsigned char display7codes [] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E, 0x7E};
  
  int a, b;
  a = value % 10;
  b = (value - a)/10;

  if (displayFlag == 0){
    PORTB = 0x0200 + display7codes[a];
  }else{
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
