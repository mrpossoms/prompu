#ifndef _I2C_UTIL
#define _I2C_UTIL

// #include <propeller.h>
#include "simplei2c.h"

// typedef struct i2c_st
// {
//   volatile int scl_mask;
//   volatile int scl_mask_inv;
//   int sda_mask;
//   int sda_mask_inv;
//   int drivescl;  /* flag to force scl if non-zero */
// } i2c;

#define I2C_RD 1
#define I2C_WR 0

#define START i2c_start(bus);
#define STOP  i2c_stop(bus);

uint8_t i2c_read_reg_byte(i2c* bus, uint8_t dev_addr, uint8_t reg);
void    i2c_read_reg_range(i2c* bus,
                           uint8_t dev_addr,
                           uint8_t reg,
                           int bytes,
                           uint8_t* buf);

void i2c_write_reg_byte(i2c* bus,
                        uint8_t dev_addr,
                        uint8_t reg,
                        uint8_t byte);
#endif
