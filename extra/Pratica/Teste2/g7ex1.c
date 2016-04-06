#include <detpic32.h>

void main(void)
{
	// Configure Timer T3 (2 Hz, interrupts disabled)
	T3CONbits.TCKPS = 7;
	PR3 = 39061;
	TMR3 = 0;
	T3CONbits.TON = 1;
	while(1)
	{
		// Wait until T3IF == 1
		while(IFS0bits.T3IF != 1);
		// Reset T3IF
		putChar('.');
		IFS0bits.T3IF = 0;
	}
}
