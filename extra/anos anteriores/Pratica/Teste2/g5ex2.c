#include <detpic32.h>

void main(void){
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0; //RB14 analog input
	AD1CHSbits.CH0SA = 14;	
	AD1CON2bits.SMPI = 0; // 1 sample converted
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 16;
	AD1CON3bits.SAMC = 16;
	AD1CON1bits.ON = 1;
	
	while(1){
		AD1CON1bits.ASAM = 1;
		while(IFS1bits.AD1IF == 0);
		printInt(ADC1BUF0,0x00030010);
		printStr("\n");
		IFS1bits.AD1IF = 0;
	}	
	
}
