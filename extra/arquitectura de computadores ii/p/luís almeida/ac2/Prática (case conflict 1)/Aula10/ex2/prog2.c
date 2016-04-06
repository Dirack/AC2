#include <detpic32.h>
#include "i2c.h"

#define ADDR_WR       ((SENS_ADDRESS << 1) | I2C_WRITE)
#define ADDR_RD       ((SENS_ADDRESS << 1) | I2C_READ)
#define SENS_ADDRESS  0x4D    // device dependent
#define TC74_CLK_FREQ 100000  // 100 KHz
#define RTR           0       // Read Temperature Command

int getTemperature(void);

int main(void){
  int temperature;
  i2c1_init(TC74_CLK_FREQ);

  while(1){
    temperature = getTemperature(); 
    printStr("O valor da temperatura: ");
    printInt10(temperature);
    printStr("\r");
    resetCoreTimer();
    while(readCoreTimer() < 5000000); // wait 250 ms
  }
}

int getTemperature(void){
  int ack, temperature;
  i2c1_start();
  ack = i2c1_send(ADDR_WR);
  ack += i2c1_send(RTR);
  i2c1_start();
  ack += i2c1_send(ADDR_RD);

  if(ack != 0){
    i2c1_stop();
    printStr("Ocorreu um erro!");
    exit(1);
  }

  temperature = i2c1_receive(I2C_NACK);
  i2c1_stop();
  return temperature;
}
