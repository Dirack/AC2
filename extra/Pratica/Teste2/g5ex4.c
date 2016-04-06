#include <detpic32.h>

int main(void){
	TRISBbits.TRISB14 = 1;
	AD1PCFGbits.PCFG14 = 0;
	AD1CHSbits.CH0SA = 14;
	AD1CON2bits.SMPI = 15; //16 samples
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 16;
	AD1CON3bits.SAMC = 16;
	AD1CON1bits.ON = 1;
	int i;
	while(1){
		AD1CON1bits.ASAM = 1;
		while(IFS1bits.AD1IF == 0);
		int *p = (int*) (&ADC1BUF0);
		for(i = 0; i < 16; i++){
			printInt(p[i*4],0x0004000A);
			printStr(" ");
		}
		printStr("\n");
		IFS1bits.AD1IF = 0;
	}
	return 0;
}
