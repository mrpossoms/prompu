#include "i2c_util.h"

i2c_t* I2C_SELECTED;

#define START(OP) i2c_start(); (OP);
#define STOP(OP)  (OP); i2c_stop();


void i2c_init(i2c_t* bus, int scl, int sda, int flags)
{
	i2c_open(&bus->prop_i2c, scl, sda, flags);
}

inline void address_slave_wr(uint8_t dev_addr)
{
	START(
		i2c_writeByte(I2C_SELECTED, dev_addr | I2C_WR)
	)
}

inline void address_slave_rd(uint8_t dev_addr)
{
	START(
		i2c_writeByte(I2C_SELECTED, dev_addr | I2C_RD)
	)
}

int i2c_readData(unsigned char *data, int count)
{
	int n = 0;
	while(--count > 0) {
		data[n] = (unsigned char) i2c_readByte(I2C_SELECTED, 0);
		n++;
	}
	data[n] = (unsigned char) i2c_readByte(I2C_SELECTED, 1);
	return n;
}

void _i2c_init_read(uint8_t dev_addr, uint8_t reg)
{
	address_slave_wr(dev_addr);
	i2c_writeByte(I2C_SELECTED, reg);
	address_slave_rd(dev_addr);
}

uint8_t i2c_read_reg_byte(uint8_t dev_addr, uint8_t reg)
{
	uint8_t byte = 0;

	_i2c_init_read(dev_addr, reg);
	STOP(
		byte = i2c_readByte(I2C_SELECTED, 1)
	)

	return byte;
}

void i2c_read_reg_range(
                        uint8_t dev_addr,
                        uint8_t reg,
                        int bytes,
                        uint8_t* buf)
{
	// config for auto-incremented addressing
	_i2c_init_read(dev_addr, reg | 0x80);
	STOP(
		i2c_readData(buf, bytes)
	)
}

void i2c_write_reg_byte(
                        uint8_t dev_addr,
                        uint8_t reg,
                        uint8_t byte)
{
	address_slave_wr(dev_addr);
	i2c_writeByte(I2C_SELECTED, reg);
	STOP(
		i2c_writeByte(I2C_SELECTED, byte)
	)
}
