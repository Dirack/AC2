#include <detpic32.h>

int main(void){

  TRISEbits.TRISE0 = 0;     // definicao de RE0 como saida
  //configuracao das entradas analogicas
  TRISBbits.TRISB14 = 1;    // Desliga saida digital de RB14
  AD1PCFGbits.PCFG14 = 0;   // Configura RB14 como entrada analogica

  //Seleccao do canal de entrada
  AD1CHSbits.CH0SA = 14;    // Escolhe o AN14 como entrada do conversor

  // configuracoes adicionais
  AD1CON1bits.SSRC = 7;     // conversion trigger selection bits: in this mode an internal counter ends sampling and starts conversion
  AD1CON1bits.CLRASAM = 1;   // Stop conversions when the first A/D converterinterruptis generated. At the same time, hardware clears the ASM bit
  AD1CON3bits.SAMC = 16;    // sample time is 16 TAD (TAD = 100 ns)
  AD1CON2bits.SMPI = 1-1;   // Interrupt is generated after XX samples replace XX by the desired number of consecutive samples (neste caso XX-1, onde XX = 1)

  AD1CON1bits.ON = 1;       // enable A/D converter. This must be the last comand of the A/D configuration sequence

  // Interrupts Configuration
  IEC1bits.AD1IE = 1;
  IPC6bits.AD1IP = 3;       // por exemplo, pois como temos apenas uma
                            // coisa a correr nao interessa o valor, tem e
                            // de ser entre 1 a 6.

  IFS1bits.AD1IF = 0;             // Reset AD1IF flag
  EnableInterrupts();             // Global Interrupt Enable

  AD1CON1bits.ASAM = 1;           // iniciar conversoes

  while(1){
    ; // do nothing (all activity is done by the ISR)
  }
}

// Interrupt Handler
void _int_(27) isr_adc(void){
/* inside of int(XXXX) Replace vector by the A/D vector number - see
"PIC32 family dat sheet" (pages 122-124)*/
  
  LATEbits.LATE0 = 0;             // Reset RE0

  // Print ADC1BUF0 value, Hexadecimal (3 digits format)
  printInt(ADC1BUF0, 0x00030010);
  printStr("\r");                 // para impressao na mesma linha
  
  LATEbits.LATE0 = 1;             // Set RE0

  // start A/D conversion
  AD1CON1bits.ASAM = 1;           // iniciar conversoes

  IFS1bits.AD1IF = 0;             // Reset AD1IF flag
}
