#include <detpic32.h>
void main(void)
{
	 //configurar timer
                    //prescaler 256 - fout=2  // fin=20000000/256 
	T3CONbits.TCKPS=7;
	PR3=39061;
	TMR3=0;
	T3CONbits.TON=1;    //enable

	IFS0bits.T3IF=0;       // reset 
	IEC0bits.T3IE=1;	//ativaçao parcial 	
	IPC3bits.T3IP=1;	//AD1IP
	EnableInterrupts();    //ativaçao global
	while(1)
	{}

}//end main

void _int_(12) isr_t3(void)
{	
	IFS0bits.T3IF=0; 
	putChar('.');	
}

