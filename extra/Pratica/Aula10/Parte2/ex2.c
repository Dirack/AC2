#include <detpic32.h>
#include "i2c.h"

#define I2C_READ 1
#define I2C_WRITE 0
#define I2C_ACK 0
#define I2C_NACK 1

#define ADDR_WR ((SENS_ADDRESS << 1) | I2C_WRITE)
#define ADDR_RD ((SENS_ADDRESS << 1) | I2C_READ)
#define SENS_ADDRESS 0x4D 	//device dependent
#define TC74_CLK_FREQ 100000 	//100Khz
#define RTR 0			// Read temperature command
int getTemperature(int *temperature)
{
	i2c1_init(TC74_CLK_FREQ);
	int ack;
	// Send Start event
	i2c1_start();
	// Send Address + WR (ADDR_WR) and copy return value to "ack" variable
	ack = i2c1_send(ADDR_WR);
	// Send Command (RTR); add return value to "ack" variable
	ack += i2c1_send(RTR);		
	// Send Start event (again)
	i2c1_start();
	// Send Address + RD (ADDR_RD); add return value to "ack" variable
	ack += i2c1_send(ADDR_RD);	
	// Test "ack" variable; if "ack" != 0 then an error has occurred; send the Stop event, print an error message and exit loop
	if(ack != 0){
		i2c1_stop();
		printf("Error!\n");
	}
	else{
		//printf("D");
		// Receive a value from slave (send NACK as argument); copy received value to "temperature" variable
		*temperature = i2c1_receive(I2C_NACK);
		//printf("E");
		// Send Stop event
		i2c1_stop();
	}	
	return ack;
}
void main(void){
	int temperature = 0;
	while(1){
	//	printf("A");	
		resetCoreTimer();
		int timer = readCoreTimer();
		while(timer <= 5000000){
			timer = readCoreTimer();
		}
	//	printf("B");
		getTemperature(&temperature);
	//	printf("C");
		printInt10(temperature);	
	}
}
