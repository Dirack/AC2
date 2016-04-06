#include <detpic32.h>
#include <ptrodrigues.h>
#define FREQ3 2

void _init_T3()
{
	T3CONbits.TCKPS = 7;
	PR3 = ((PBCLK/256)/FREQ3)-1;
	TMR3 = 0;
	T3CONbits.TON = 1;
	IFS0bits.T3IF = 0;
	IEC0bits.T3IE = 1;
	IPC3bits.T3IP = 2;
}

void _int_(12) isr_T3(void)
{
	//THE CODE COMMENTED IMPLEMENT A OUT FREQUENCY OF 1Hz
	/*
	static uint timer = 0;
	timer++;
	if(timer % 2 == 0)
	*/putChar('.');
	IFS0bits.T3IF = 0;
}

int main(void)
{
	_init_T3();
	EnableInterrupts();
	while(true);
	return 0;
}
