#include "i2c_util.h"

#define ALL_HIGH sda_high();\
                 scl_high();

#define ALL_LOW sda_low();\
                scl_low();

_COGMEM volatile int scl_mask;
_COGMEM volatile int scl_mask_inv;
_COGMEM int sda_mask;
_COGMEM int sda_mask_inv;

_NAKED void scl_low()
{
  OUTA &= scl_mask_inv;
  DIRA |= scl_mask;
}

_NAKED void sda_low()
{
  OUTA &= sda_mask_inv;
  DIRA |= sda_mask;
}

_NAKED void scl_high()
{
  OUTA &= scl_mask_inv;
  DIRA &= scl_mask_inv;
}

_NAKED void sda_high()
{
  DIRA &= sda_mask_inv;
}

_NAKED void i2c_start()
{
  ALL_HIGH
  sda_low();
  scl_low();
}

_NAKED void i2c_stop()
{
  ALL_LOW
  scl_high();
#ifndef __PROPELLER_CMM__
  /* second scl_high call gives delay for sda_high to make timing */
  scl_high();
#endif
  sda_high();
}

_NAKED void i2c_open(int sclPin, int sdaPin)
{
  scl_mask = (1 << sclPin);
  sda_mask = (1 << sdaPin);
  scl_mask_inv = ~(1 << sclPin);
  sda_mask_inv = ~(1 << sdaPin);
  i2c_stop();
}

_NAKED int i2c_writeByte(int byte)
{
  int result;
  int count = 8;

  /* send the byte, high bit first */
  do {
    if (byte & 0x80)
      DIRA &= sda_mask_inv;
    else
      DIRA |= sda_mask;
    scl_high();
    byte <<= 1;
    scl_low();
  } while(--count > 0);
  DIRA &= sda_mask_inv;
  /* get ack */
  scl_high();
  result = (INA & sda_mask);
  scl_low();
  return result != 0;
}

_NAKED int i2c_readByte(int ackState)
{
  int byte = 0;
  int count = 8;

  DIRA &= sda_mask_inv;

  for (count = 8; --count >= 0; ) {
    byte <<= 1;
    scl_high();
    byte |= (INA & sda_mask) ? 1 : 0;
    while(!(INA & scl_mask));  /* clock stretching */
    /* scl_low(); // slow */
    OUTA &= scl_mask_inv;
    DIRA |= scl_mask;
  }

  /* acknowledge */
  if (ackState)
    DIRA &= sda_mask_inv;
  else
    DIRA |= sda_mask;
  scl_high();
  scl_low();

  return byte;
}

_NAKED int i2c_readData(unsigned char *data, int count)
{
  int n = 0;
  while(--count > 0) {
    data[n] = (unsigned char) i2c_readByte(0);
    n++;
  }
  data[n] = (unsigned char) i2c_readByte(1);
  return n;
}

_NAKED void _i2c_init_read(uint8_t dev_addr, uint8_t reg)
{
  i2c_writeByte(dev_addr | I2C_WR);
  i2c_writeByte(reg);
  START
  i2c_writeByte(dev_addr | I2C_RD);
}

_NAKED uint8_t i2c_read_reg_byte(uint8_t dev_addr, uint8_t reg)
{
  uint8_t byte = 0;

  START
  _i2c_init_read(dev_addr, reg);
  byte = i2c_readByte(1);
  STOP

  return byte;
}

_NAKED void i2c_read_reg_range(
                        uint8_t dev_addr,
                        uint8_t reg,
                        int bytes,
                        uint8_t* buf)
{
  START
  // config for auto-incremented addressing
  _i2c_init_read(dev_addr, reg | 0x80);

  int last = bytes - 1;
  i2c_readData(buf, bytes);
  STOP
}

_NAKED void i2c_write_reg_byte(
                        uint8_t dev_addr,
                        uint8_t reg,
                        uint8_t byte)
{
  START
  i2c_writeByte(dev_addr | I2C_WR);
  i2c_writeByte(reg);
  i2c_writeByte(byte);
  STOP
}
