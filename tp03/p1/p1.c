#include <detpic32.h>
void main(void)
{
	TRISEbits.TRISE0 = 0; // RD0 configured as output
	while(1)
	{
		while(readCoreTimer() < (FREQ/2)); // half period = 0.5s
		resetCoreTimer();
		LATEbits.LATE0 = !LATEbits.LATE0;
	}
}
