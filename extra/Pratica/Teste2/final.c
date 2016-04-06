#include <detpic32.h>

void send2displays(unsigned char value){
	unsigned char digit_low,digit_high;
	static unsigned char displayFlag = 0;
	static const unsigned char display7Scodes[] =  {0x77,0x41,0x3B,0x6B,0x4D,0x6E,0x7E,0x43,0x7F,0x6F,0x5F,0x7C,0x36,0x79,0x3E,0x1E};
	
	digit_low = value & 0x0F;
	digit_high = value >> 4;

	if(displayFlag == 0){
		LATBbits.LATB9 = 1;
		LATBbits.LATB8 = 0;
		LATB = LATB & 0xFF00;
		LATB = LATB | display7Scodes[digit_low];
	}	
	else{
		LATBbits.LATB9 = 0;
		LATBbits.LATB8 = 1;
		LATB = LATB & 0xFF00;
		LATB = LATB | display7Scodes[digit_high];
	}
	displayFlag = !displayFlag;
}
void setPWM(unsigned int duty){
	OC1RS = ((PR3+1)*duty)/100;
}
volatile unsigned int value2display;
void main(void){
	//Configurations
	TRISB = TRISB & 0xFC00;
	TRISE = TRISE | 0xF0;
	
	//ADC 
	TRISBbits.TRISB14 = 1; // disconnect output
	AD1PCFGbits.PCFG14 = 0;
	AD1CHSbits.CH0SA = 14;
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON2bits.SMPI = 0;
	AD1CON3bits.SAMC = 16;
	AD1CON1bits.ON = 1;
	AD1CON1bits.ASAM = 1;

	IFS1bits.AD1IF = 0;
	IPC6bits.AD1IP = 2;
	IEC1bits.AD1IE = 1;
	
	//Timer T3
	T3CONbits.TCKPS = 2;
	PR3 = 49999;
	TMR3 = 0;
	T3CONbits.TON = 1;

	IFS0bits.T3IF = 0;
	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;


	//Timer T1
	T1CONbits.TCKPS = 3;
	PR1 = 19530;
	TMR1 = 0;
	T1CONbits.TON = 1;
	
	IFS0bits.T1IF = 0;
	//	IEC0bits.T1IE = 1;
	IPC1bits.T1IP = 2;

	//OC
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	OC1CONbits.ON = 1;
	EnableInterrupts();
	while(1)
	{
		if(PORTEbits.RE4 == 1 && PORTEbits.RE5 == 0){
			IEC0bits.T1IE = 1;
			setPWM(0);	
		}
		else if(PORTEbits.RE5 == 1 && PORTEbits.RE4 == 0){
			IEC0bits.T1IE = 0;
			setPWM(50);
		}
		else{ 
			IEC0bits.T1IE = 0;
			setPWM(100);
		}
	}
}
void _int_(27) isr_adc(void)
{
	int value = (ADC1BUF0*33+511)/1023;
	char decimal = (value/10)*16+value%10;
	value2display = decimal;
	IFS1bits.AD1IF = 0;
}
void _int_(4) isr_T1(void)
{
	AD1CON1bits.ASAM = 1;
	IFS0bits.T1IF = 0;
}
void _int_(12) isr_T3(void)
{
	send2displays(value2display);
	IFS0bits.T3IF = 0;
}
