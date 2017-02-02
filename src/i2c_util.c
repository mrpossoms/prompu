#include "i2c_util.h"

void _i2c_init_read(i2c* bus, uint8_t dev_addr, uint8_t reg)
{
  i2c_writeByte(bus, dev_addr | I2C_WR);
  i2c_writeByte(bus, reg);
  START
  i2c_writeByte(bus, dev_addr | I2C_RD);
}

uint8_t i2c_read_reg_byte(i2c* bus, uint8_t dev_addr, uint8_t reg)
{
  uint8_t byte = 0;

  START
  _i2c_init_read(bus, dev_addr, reg);
  byte = i2c_readByte(bus, 1);
  STOP

  return byte;
}

void i2c_read_reg_range(i2c* bus,
                        uint8_t dev_addr,
                        uint8_t reg,
                        int bytes,
                        uint8_t* buf)
{
  START
  // config for auto-incremented addressing
  _i2c_init_read(bus, dev_addr, reg | 0x80);

  int last = bytes - 1;
  for(int i = 0; i < bytes; ++i)
  {
    buf[i] = i2c_readByte(bus, i == last);
  }

  STOP
}

void i2c_write_reg_byte(i2c* bus,
                        uint8_t dev_addr,
                        uint8_t reg,
                        uint8_t byte)
{
  START
  i2c_writeByte(bus, dev_addr | I2C_WR);
  i2c_writeByte(bus, reg);
  i2c_writeByte(bus, byte);
  STOP
}
