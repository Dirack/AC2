#include <detpic32.h>
#include "i2c.h"
int main(void)
{
	int ack, temperature;
	i2c1_init(TC74_CLK_FREQ);
	while(1)
	{
		// Send Start event
		i2c1_start();
		// Send Address + WR (ADDR_WR); copy return value to "ack" variable
		ack = i2c1_send(ADDR_WR);
		// Send Command (RTR); add return value to "ack" variable
		ack += i2c1_send(RTR);
		// Send Start event (again)
		i2c1_start();
		// Send Address + RD (ADDR_RD); add return value to "ack" variable
		ack += i2c1_send(ADDR_RD);
		// Test "ack" variable; if "ack" != 0 then an error has occurred; send the Stop event, print an error message and exit loop
		if(ack != 0)
		{
			printf("Ocorreu um erro, valor de ack = 0!!!\n");
			i2c1_stop();
			break;
		}
		// Receive a value from slave (send NACK as argument); copy received value to "temperature" variable
		temperature = i2c1_receive(I2C_NACK);
		// Send Stop event
		i2c1_stop();
		// Print "temperature" variable (syscall printInt10)
		printInt10(temperature);
		printf("\r");
		// Wait 250 ms
		delay(250);
	}
	return 0;
}

