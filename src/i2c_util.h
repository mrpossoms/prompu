#ifndef _I2C_UTIL
#define _I2C_UTIL

#include "boards.h"

typedef struct {
#ifdef TARGET_IS_PROPELLER
	i2c prop_i2c;
#endif
	int i2c_module;
} i2c_t;

extern i2c_t* I2C_SELECTED;

void    i2c_init(i2c_t* bus, int scl, int sda, int flags);
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
