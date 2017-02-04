#ifndef _I2C_UTIL
#define _I2C_UTIL

#include <propeller.h>
// #include "simplei2c.h"

#define I2C_RD 1
#define I2C_WR 0

#define START i2c_start();
#define STOP  i2c_stop();

void    i2c_open(int sclPin, int sdaPin);
uint8_t i2c_read_reg_byte(uint8_t dev_addr, uint8_t reg);
void    i2c_read_reg_range(
                           uint8_t dev_addr,
                           uint8_t reg,
                           int bytes,
                           uint8_t* buf);
void i2c_write_reg_byte(
                        uint8_t dev_addr,
                        uint8_t reg,
                        uint8_t byte);
#endif
