#include <detpic32.h>

//declaracao de funcao delay e send2displays
void send2displays(unsigned char);
void set_freq_1(int);
void set_freq_3(int);
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
  AD1CON2bits.SMPI = 8-1;   // Interrupt is generated after XX samples replace XX by the desired number of consecutive samples (neste caso XX-1, onde XX = 1)

  AD1CON1bits.ON = 1;       // enable A/D converter. This must be the last comand of the A/D configuration sequence

  // Configuracao de Timers
  set_freq_3(100);
  TMR3 = 0;
  T3CONbits.TON = 1;
  set_freq_1(4);
  TMR1 = 0;
  T1CONbits.TON = 1;
  
  // Configuracao de interrupcoes
  IFS0bits.T3IF = 0;
  IPC3bits.T3IP = 2;
  IEC0bits.T3IE = 1;
  IFS0bits.T1IF = 0;
  IPC1bits.T1IP = 2;
  IEC0bits.T1IE = 1;


  // Interrupts Configuration
  IEC1bits.AD1IE = 1;
  IPC6bits.AD1IP = 3;       // por exemplo, pois como temos apenas uma
                            // coisa a correr nao interessa o valor, tem e
                            // de ser entre 1 a 6.

  IFS1bits.AD1IF = 0;             // Reset AD1IF flag
  IFS0bits.T1IF = 0;              // Reset timer T1 Flag
  IFS0bits.T3IF = 0;              // Reset timer T3 Flag
  EnableInterrupts();             // Global Interrupt Enable

  while(1);
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

void _int_(4) isr_T1(void){
  AD1CON1bits.ASAM = 1;           // iniciar conversoes
  IFS0bits.T1IF = 0;              // Reset T1 Timer Flag
}

void _int_(12) isr_T3(void){
  send2displays(tmedio);
  IFS0bits.T3IF = 0;              // Reset T3 Timer Flag
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

void set_freq_3(int Hz){
  static int shift [8] = {0, 1, 2, 3, 4, 5, 6, 8};
  int i, c;

  c = (PBCLK + Hz/2)/Hz;
  for (i = 0; i < 8 && (c >> shift[i]) > 65536; i++)
    ;
  if (i == 0)
    exit(1);
  PR3 = (c >> shift[i]) - 1;
  T3CONbits.TCKPS = i;
}

void set_freq_1(int Hz){
  static int shift [8] = {0, 1, 2, 3, 4, 5, 6, 8};
  int i, c;

    c = (PBCLK + Hz/2)/Hz;
    for (i = 0; i < 8 && (c >> shift[i]) > 65536; i++)
      ;
    if (i == 0)
      exit(1);
    PR1 = (c >> shift[i]) - 1;
    T1CONbits.TCKPS = i;
}

