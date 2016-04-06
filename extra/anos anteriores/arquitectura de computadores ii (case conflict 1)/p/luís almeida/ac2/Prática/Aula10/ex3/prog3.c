#include <detpic32.h>
#include "i2c.h"

#define ADDR_WR       ((SENS_ADDRESS << 1) | I2C_WRITE)
#define ADDR_RD       ((SENS_ADDRESS << 1) | I2C_READ)
#define SENS_ADDRESS  0x4D
#define TC74_CLK_FREQ 100000
#define RTR           0

// declaracao de funcoes
volatile unsigned int value2display = 0;
volatile unsigned int dot;
volatile unsigned int valueBinary = 0;
void configureAll(void);
void set_freq_1(int);
void set_freq_3(int);
void setPWM(int);
void send2displays(int);
int getTemperature(void);

// inicio da main
int main(void){

  static const unsigned char pwmValues [] = {3, 15, 40, 90};
  int dc;
  i2c1_init(TC74_CLK_FREQ);

  configureAll();
  EnableInterrupts();

  while(1){
    if (PORTEbits.RE4 == 0 && PORTEbits.RE5 == 0){
      IEC0bits.T1IE = 1;
      setPWM(0);
      dot = 1;
    }else if (PORTEbits.RE4 == 1 && PORTEbits.RE5 == 0){
      IEC0bits.T1IE = 0;
      setPWM(100);
      dot = 1;
    }else if (PORTEbits.RE4 == 0 && PORTEbits.RE5 == 1){
      IEC0bits.T1IE = 0;
      dc = (PORTE&0xC0) >> 6;
      value2display = pwmValues[dc];
      setPWM(pwmValues[dc]);
      dot = 0;
    }else if (PORTEbits.RE4 == 1 && PORTEbits.RE5 == 1){
      IEC0bits.T1IE = 0;
      dot = 0;
      setPWM(0);
      value2display = getTemperature();
      resetCoreTimer();
      while(readCoreTimer() < 5000000); // wait 250 ms
    }
  }
  return 0;
}

void configureAll(void){
  // ADC setup
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CON3bits.SAMC = 16;

  TRISBbits.TRISB14 = 1;
  AD1PCFGbits.PCFG14 = 0;
  AD1CHSbits.CH0SA = 14;
  AD1CON2bits.SMPI = 8-1;
  AD1CON1bits.ON = 1;

  // timer 1
  set_freq_1(4);
  TMR1 = 0;
  T1CONbits.TON = 1;

  // timer 3
  set_freq_3(100);
  TMR3 = 0;
  T3CONbits.TON = 1;

  // interrupt handlers
  IEC1bits.AD1IE = 1;
  IPC6bits.AD1IP = 1;

  IEC0bits.T1IE = 1;
  IPC1bits.T1IP = 2;
  IEC0bits.T3IE = 1;
  IPC3bits.T3IP = 3;
  
  // ports setup
  TRISB = 0xFC00;
  TRISE = 0x00F0;
}

// int adc
void _int_(27) isr_adc(void){
  int *p = (int *)(&ADC1BUF0);
  int a, soma = 0, media = 0;

  for (a = 0; a < 8; a++){
    printInt(p[a*4], 0x0004000A);
    soma += p[a*4];
    printStr(" ");
  }

  printStr("media = ");
  media = soma / 8;
  printInt(media, 0x0004000A);
  printStr(" Vmed = ");
  value2display = (media * 33)/1023;
  printInt(value2display, 0x0003000A);
  printStr("\r");

  IFS1bits.AD1IF = 0;
}

// int T1
void _int_(4) isr_T1(void){
  AD1CON1bits.ASAM = 1;
  IFS0bits.T1IF = 0;
}

// int T3
void _int_(12) isr_T3(void){
  send2displays(value2display);
  IFS0bits.T3IF = 0;
}

// set freq 1
void set_freq_1(int Hz){
  static int shift [] = {0, 3, 6, 8};
  int i, c;

  c = (PBCLK + Hz/2)/Hz;
  for (i = 0; i < 8 && (c >> shift[i]) > 65536; i++);
  if (i == 8)
    exit(1);

  PR1 = (c >> shift[i]) - 1;
  T1CONbits.TCKPS = i;
}

// set freq 3
void set_freq_3(int Hz){
  static int shift [] = {0, 1, 2, 3, 4, 5, 6, 8};
  int i, c;

  c = (PBCLK + Hz/2)/Hz;
  for (i = 0; i < 8 && (c >> shift[i]) > 65536; i++);
  if (i == 8)
    exit(1);

  PR3 = (c >> shift[i]) - 1;
  T3CONbits.TCKPS = i;
}

// duty cycle
void setPWM(int dutyCycle){
  if (dutyCycle < 0 || dutyCycle > 100)
    exit(1);

  OC1RS = dutyCycle * (PR3 + 1)/100;

  OC1CONbits.OCM = 6;
  OC1CONbits.OCTSEL = 1;
  OC1CONbits.ON = 1;
}

void send2displays(int value){
  static unsigned char displayFlag = 0;
  static const unsigned char display7codes [] = {0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F, 0x5F, 0x7C, 0x36, 0x79, 0x3E, 0x1E, 0xFE};

  int a, b;

  a = value % 10;
  b = (value - a) / 10;
  if (displayFlag == 0){
    PORTB = 0x0200 + display7codes[a];
  }else{
    if (dot == 1)
      PORTB = 0x0100 + display7codes[b] + 0x80;
    else
      PORTB = 0x0100 + display7codes[b];
  }
  displayFlag = ~displayFlag;
}

int getTemperature(void){
  int ack, temperature;
  i2c1_start();
  ack = i2c1_send(ADDR_WR);
  ack += i2c1_send(RTR);
  i2c1_start();
  ack += i2c1_send(ADDR_RD);

  if (ack != 0){
    i2c1_stop();
    printStr("Ocorreu um erro!");
    exit(1);
  }

  temperature = i2c1_receive(I2C_NACK);
  i2c1_stop();
  return temperature;
}
