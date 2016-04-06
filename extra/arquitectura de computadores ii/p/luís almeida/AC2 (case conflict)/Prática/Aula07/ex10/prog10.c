#include <detpic32.h>

// declaracao de funcoes
volatile unsigned int value2display = 0;
volatile unsigned int dot;
volatile unsigned int valueBinary = 0;
void configureAll(void);
void set_freq_1(int);
void set_freq_3(int);
void setPWM(int);
void send2displays(unsigned char);
void binaryCounter(void);

// inicio da main
int main(void){
  
  const static unsigned char pwmValues [] = {3, 15, 40, 90};
  int dc;

  configureAll();
  EnableInterrupts();       // Global Interrupts enable

  while(1){
    if ((PORTEbits.RE4 == 0) && (PORTEbits.RE5 == 0)){
      valueBinary = 0;
      LATE = valueBinary;
      IEC0bits.T1IE = 1;        // enable T1 interrupts
      setPWM(0);
      dot = 1;
    }else if ((PORTEbits.RE4 == 1) && (PORTEbits.RE5 == 0)){
      valueBinary = 0;
      LATE = valueBinary;
      IEC0bits.T1IE = 0;        // disable T1 interrupts
      setPWM(100);
      dot = 1;
    }else if ((PORTEbits.RE4 == 0) && (PORTEbits.RE5 == 1)){
      valueBinary = 0;
      LATE = valueBinary;
      IEC0bits.T1IE = 0;        // disable T1 interrupts
      dc = (PORTE&0xC0) >> 6;   // read PORTE RE6 and RE7
      setPWM(pwmValues[dc]);
      value2display = pwmValues[dc];
      dot = 0;
    }else if ((PORTEbits.RE4 == 1) && (PORTEbits.RE5 == 1)){
      IEC0bits.T1IE = 0;        // disable T1 interrupts
      setPWM(0);
      binaryCounter();
    }
  }
  return 0;
}

void configureAll(void){

  // ADC Setup
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CON3bits.SAMC = 16;
  
  TRISBbits.TRISB14 = 1;      // RB14 input
  AD1PCFGbits.PCFG14 = 0;     // RB14 Analog
  AD1CHSbits.CH0SA = 14;      // Select AN14 in mux
  AD1CON2bits.SMPI = 7;       // 8 conversion
  AD1CON1bits.ON = 1;         // enable ADC

  // Timer 1 setup
  set_freq_1(4);
  TMR1 = 0;
  T1CONbits.TON = 1;

  // Timer 3 setup
  set_freq_3(100);
  TMR3 = 0;
  T3CONbits.TON = 1;

  // Interrupt setup
  IPC6bits.AD1IP = 1;
  IEC1bits.AD1IE = 1;

  IEC0bits.T1IE = 1;
  IPC1bits.T1IP = 2;
  IEC0bits.T3IE = 1;
  IPC3bits.T3IP = 3;

  // Port setup
  TRISB = 0xFC00;   // definicao de ports RB0 a RB9 como saida
  TRISE = 0x00F0;   // definicao de ports RE0 a RE3 como saida e RE4 a RE7
                    // como entrada
}

// interrupt handlers
// ADC
void _int_(27) isr_adc(void){
  int *p = (int *)(&ADC1BUF0);
  int a, soma = 0, media = 0;

  for (a = 0; a < 8; a++){
    printInt(p[a * 4], 0x0004000A);
    soma += p[a * 4];
    printStr(" ");
  }

  printStr("media = ");
  media = soma/8;
  printInt(media, 0x0004000A);
  printStr(" Vmed = ");
  value2display = (media * 33) / 1023;
  printInt(value2display, 0x0002000A);
  printStr("\r");

  IFS1bits.AD1IF = 0;     // Reset AD1IF Flag
}

void _int_(4) isr_T1(void){
  AD1CON1bits.ASAM = 1;             // iniciar conversoes
  IFS0bits.T1IF = 0;                // Reset T1 Timer Flag
}

void _int_(12) isr_T3(void){
  send2displays(value2display);   
  IFS0bits.T3IF = 0;                // Reset T3 Timer Flag
}

void set_freq_1(int Hz){
  static int shift [8] = {0, 1, 2, 3, 4, 5, 6, 8};
  int i, c;

  c = (PBCLK + Hz/2)/Hz;
  for (i = 0; i < 8 && (c >> shift[i]) > 65536; i++);
  if (i == 0)
    exit(1);
  PR1 = (c >> shift[i]) - 1;
  T1CONbits.TCKPS = i;
}

void set_freq_3(int Hz){
  static int shift [8] = {0, 1, 2, 3, 4, 5, 6, 8};
  int i, c;

  c = (PBCLK + Hz/2)/Hz;
  for (i = 0; i < 8 && (c >> shift[i]) > 65536; i++);
  if (i == 8)
    exit(1);

  PR3 = (c >> shift[i]) - 1;
  T3CONbits.TCKPS = i;
}

void setPWM(int dutyCycle){
  if (dutyCycle < 0 || dutyCycle > 100)
    exit(1);

  OC1RS = dutyCycle * (PR3+1)/100;        // OC1 PWM constant

  // PWM T3
  OC1CONbits.OCM = 6;       // PWM mode on OC1
  OC1CONbits.OCTSEL = 1;    // Select Timer 3
  OC1CONbits.ON = 1;        // enable OC1
}

void send2displays(unsigned char value){
  static unsigned char displayFlag = 0;
  static const unsigned char display7codes [] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E, 0x7E};

  int a, b;
  a = value% 10;
  b = (value - a)/10;

  if (displayFlag == 0){
    PORTB = 0x0200 + display7codes[a];
  }else{
    if(dot == 1)
      PORTB = 0x0100 + display7codes[b] + 0x80;
    else
      PORTB = 0x0100 + display7codes[b];
  }
  displayFlag = ~displayFlag;
}

void binaryCounter(void){
  resetCoreTimer();
  while(readCoreTimer() < 5000000);
  if (valueBinary == 16){
    valueBinary = 0;
    LATE = valueBinary;
    value2display = valueBinary;
    valueBinary++;
  }else{
    LATE = valueBinary;
    value2display = valueBinary;
    valueBinary++;
  }
}
