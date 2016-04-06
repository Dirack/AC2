#include <detpic32.h>

void main(void)
{
	// Configure Timer T3, interrupts
	T3CONbits.TCKPS = 7;
	PR3 = 39061;
	TMR3 = 0;
	T3CONbits.TON = 1;

	IPC3bits.T3IP = 2;
	IFS0bits.T3IF = 0;
	IEC0bits.T3IE = 1;
	EnableInterrupts();
	while(1);
}
void _int_(12) isr_T3(void) // Replace VECTOR by the timer T3
	// vector number
{
	putChar('.');
	// Reset T3 interrupt flag
	IFS0bits.T3IF = 0;
}
