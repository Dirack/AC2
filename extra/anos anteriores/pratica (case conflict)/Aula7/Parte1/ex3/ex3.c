#include<detpic32.h>
void main(void)
{
	// Configure Timer T3, interrupts
	T3CONbits.TCKPS = 7; // 1:256 prescaler
	PR3 = 39062; // 
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the timer configuration sequence)
	IPC3bits.T3IP = 1;
	IEC0bits.T3IE = 1;
	EnableInterrupts();
	while(1);
}
int flag = 0;
void _int_(12) isr_T3(void) // Replace VECTOR by the timer T3
	// vector number
{
	if(flag == 0){
		putChar('.');
	}
	// Reset T3 interrupt flag
	flag = !flag;
	IFS0bits.T3IF = 0;
}
