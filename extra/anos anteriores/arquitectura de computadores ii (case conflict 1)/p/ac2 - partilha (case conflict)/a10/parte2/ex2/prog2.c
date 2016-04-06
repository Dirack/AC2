#include <detpic32.h>
#include "i2c.h"
#define var = 0
int getTemperature(int *temperature);
int main (void)
{
	int *temperature;
	delay(250);
	if(var == 4)
	{
		printInt10(temperature);
		var = 0;
	}
	temperature = getTemperature(&temperature);
	var++;
	return 0;
}

int getTemperature(int *ptemperature)
{
	int ack;
	// Send Start event
	i2c1_start();
	// Send Address + WR (ADDR_WR) and copy return value to "ack" variable
	ack = i2c1_send(ADDR_WR);
	i2c1_init(TC74_CLK_FREQ);
		
	i2c1_start();
	ack = i2c1_send(ADDR_WR);
	ack += i2c1_send(RTR);
	i2c1_start();
	ack += i2c1_send(ADDR_RD);
	if(ack != 0)
	{
		printf("Ocorreu um erro, valor de ack = 0!!!\n");
		i2c1_stop();
	}
	*ptemperature = i2c1_receive(I2C_NACK);
	i2c1_stop();
	delay(250);

	// Send Stop event
	i2c1_stop();
	return ack;
}
