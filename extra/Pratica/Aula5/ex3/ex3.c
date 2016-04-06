#include <detpic32.h>

int main(void){
	volatile int aux;
	
	TRISBbits.TRISB14 = 1; 	//RB14 digital output disconnect
	AD1PCFGbits.PCFG14 = 0; //RB14 configured as analog input (AN4)
	AD1CHSbits.CH0SA = 14; 	//Selects AN14 as input for the A/D converter
	AD1CON2bits.SMPI = 0;	//1 sample converted and stored in ADC1BUF0
	
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	
	AD1CON1bits.ON = 1;	//Enable A/D

	TRISEbits.TRISE0 = 0;

	while(1){
		LATEbits.LATE0 = 1;	//Set RE0
		AD1CON1bits.ASAM = 1;	//Start conversion
		while(IFS1bits.AD1IF == 0);	//Wait while conversion not done
		LATEbits.LATE0 = 0;
		aux = ADC1BUF0;
		IFS1bits.AD1IF = 0;	//Reset Interruption Flag bit		
	}
} 
