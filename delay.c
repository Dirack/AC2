#include <detpic32.h>
#include <ptrodrigues.h>
 void main(void)
{
	while(1)
	{
		resetCoreTimer();
		delay(1);
		printInt(readCoreTimer(), 10 + (10 << 16));
		printStr("\r\n");
	}
}
