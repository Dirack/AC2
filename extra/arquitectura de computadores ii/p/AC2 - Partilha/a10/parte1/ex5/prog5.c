#include <detpic32.h>

void i2c1_init(unsigned int clock_freq);
void i2c1_start(void);
void i2c1_stop(void);
int i2c1_send(unsigned char value);

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

void i2c1_start(void)
{
	// Activate Start event (I2C1CON, bit SEN)
	I2C1CONbits.SEN = 1;
	// Wait for completion of the Start event (I2C1CON, bit SEN)
	while(I2C1CONbits.SEN == 1);
}

void i2c1_stop(void)
{
	// Wait until the lower 5 bits of I2CxCON are all 0 (the lower 5 bitsof I2CxCON must be 0 before attempting to set the PEN bit)
	while((I2C1CON & 0x1F) != 0);
	// Activate Stop event (I2C1CON, bit PEN)
	I2C1CONbits.PEN = 1;	
	// Wait for completion of the Stop event (I2C1CON, bit PEN)
	while(I2C1CONbits.SEN == 1);
}

int i2c1_send(unsigned char value)
{
	// Copy "value" to I2C1TRN register
	I2C1TRN = value;
	// Wait while master transmission is in progress (8 bits + ACK\) (I2C1STAT, bit TRSTAT – transmit status bit)
	while(I2C1STATbits.TRSTAT==0);
	// Return acknowledge status bit (I2C1STAT, bit ACKSTAT)
	return I2C1STATbits.ACKSTAT;
}
