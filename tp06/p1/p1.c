#include <detpic32.h>
#include <ptrodrigues.h> 
#define FREQ3 2

void init_T3(void)
{
	T3CONbits.TCKPS = 7;
	PR3 = ((PBCLK/256)/FREQ3)-1;
	TMR3 = 0;
	T3CONbits.TON = 1;
}

int main(void)
{
	init_T3();
	while(true)
	{
		while(IFS0bits.T3IF == 0);
		putChar('.');
		IFS0bits.T3IF = 0;
	}
	return 0;
}
