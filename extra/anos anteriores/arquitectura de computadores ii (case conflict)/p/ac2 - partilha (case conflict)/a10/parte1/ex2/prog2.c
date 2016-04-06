#include <detpic32.h>

int main(void)
{
return 0;
}

void i2c1_init(unsigned int clock_freq)
{
	//Config BAUDRATE
	//I2CxBRG = (fPBCLK + fSCL) / (2 * fSCL) - 1
	I2C1BRG = (20000000 + clock_freq) / (2 * clock_freq) - 1;

	// Enable I2C1 module
	I2C1CONbits.ON = 1;
}
