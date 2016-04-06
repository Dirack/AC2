// i2c.h
#ifndef I2C_H
#define I2C_H

// declare symbols here (READ, WRITE, ...)
#define I2C_READ      1
#define I2C_WRITE     0
#define I2C_ACK       0
#define I2C_NACK      1

// declare functions prototypes
void i2c1_init (unsigned int);
void i2c1_start(void);
void i2c1_stop(void);
int i2c1_send(unsigned char);
char i2c1_receive(char);

#endif
