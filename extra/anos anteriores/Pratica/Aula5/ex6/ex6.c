#include <detpic32.h>

int main(void){
	TRISBbits.TRISB14 = 1; 	//RB14 digital output disconnect
	AD1PCFGbits.PCFG14 = 0; //RB14 configured as analog input (AN4)
	AD1CHSbits.CH0SA = 14; 	//Selects AN14 as input for the A/D converter
	AD1CON2bits.SMPI = 3;	//1 sample converted and stored in ADC1BUF0
	
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	
	AD1CON1bits.ON = 1;	//Enable A/D
	int i, aux,val = 0;
	while(1){
		AD1CON1bits.ASAM = 1;	//Start conversion
		while(IFS1bits.AD1IF == 0);	//Wait while conversion not done
		int *p = (int *) (&ADC1BUF0);
		aux = 0;
		val = 0;
		for(i = 0; i < 16; i++){
			printInt(p[i*4],0x0004000A);
			printStr(" ");
			aux = p[i*4];
			val += (aux*33+511)/1023;
		}
		printStr("\n");
		
		val = val/4;
		printInt(val, 10);
		printStr("\n");
		IFS1bits.AD1IF = 0;	//Reset Interruption Flag bit
	
	}
	return 0;	
}
