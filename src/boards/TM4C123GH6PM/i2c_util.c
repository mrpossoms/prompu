#include "i2c_util.h"

i2c_t* I2C_SELECTED;

void i2c_init(i2c_t* bus, int scl, int sda, int flags)
{
	SysCtlPeripheralEnable(flags);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);

	uint32_t port_base = GPIO_PORTA_BASE;

	switch(flags)
	{
		case SYSCTL_PERIPH_GPIOA : port_base = GPIO_PORTA_BASE; break;
		case SYSCTL_PERIPH_GPIOB : port_base = GPIO_PORTB_BASE; break;
		case SYSCTL_PERIPH_GPIOC : port_base = GPIO_PORTC_BASE; break;
		case SYSCTL_PERIPH_GPIOD : port_base = GPIO_PORTD_BASE; break;
		case SYSCTL_PERIPH_GPIOE : port_base = GPIO_PORTE_BASE; break;
		case SYSCTL_PERIPH_GPIOF : port_base = GPIO_PORTF_BASE; break;
	}

	GPIOPinConfigure(GPIO_PA6_I2C1SCL);
	GPIOPinConfigure(GPIO_PA7_I2C1SDA);

	GPIOPinTypeI2CSCL(port_base, scl);
	GPIOPinTypeI2C(port_base, sda);

	I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);

	//clear I2C FIFOs
	HWREG(I2C1_BASE + I2C_O_FIFOCTL) = 80008000;
}

uint8_t i2c_read_reg_byte(uint8_t dev_addr, uint8_t reg)
{
	dev_addr >>= 1; // Tiva assumes 7bit address ignoring RW bit

	// address the slave device
	I2CMasterSlaveAddrSet(I2C1_BASE, dev_addr, false);
	I2CMasterDataPut(I2C1_BASE, reg);

	// write the register address to read from
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C1_BASE)); // wait for idle
	 
	// address slave again
	I2CMasterSlaveAddrSet(I2C1_BASE, dev_addr, true);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusy(I2C1_BASE)); // wait for idle
	 
	return I2CMasterDataGet(I2C1_BASE); // read byte
}

void i2c_read_reg_range(
                        uint8_t dev_addr,
                        uint8_t reg,
                        int bytes,
                        uint8_t* buf)
{
	dev_addr >>= 1; // Tiva assumes 7bit address ignoring RW bit
	reg |= 0x80;    // Setting the Msb of the register enables auto incrementing


	// address the slave device
	I2CMasterSlaveAddrSet(I2C1_BASE, dev_addr, false);
	I2CMasterDataPut(I2C1_BASE, reg);

	// write the register address to read from
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C1_BASE)); // wait for idle
	 
	// address slave again
	I2CMasterSlaveAddrSet(I2C1_BASE, dev_addr, true);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
	while(I2CMasterBusy(I2C1_BASE)); // wait for idle 
	buf[0] = I2CMasterDataGet(I2C1_BASE); // read byte

	for(int i = 1; i < bytes - 1; ++i)
	{
		I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
		while(I2CMasterBusy(I2C1_BASE)); // wait for idle 
		buf[i] = I2CMasterDataGet(I2C1_BASE); // read byte
	}

	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	while(I2CMasterBusy(I2C1_BASE)); // wait for idle 
	buf[bytes - 1] = I2CMasterDataGet(I2C1_BASE); // read byte
}

void i2c_write_reg_byte(
                        uint8_t dev_addr,
                        uint8_t reg,
                        uint8_t byte)
{
	dev_addr >>= 1; // Tiva assumes 7bit address ignoring RW bit

	I2CMasterSlaveAddrSet(I2C1_BASE, dev_addr, false);
	I2CMasterDataPut(I2C1_BASE, reg);

	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START); 
	while(I2CMasterBusy(I2C1_BASE));

	I2CMasterDataPut(I2C1_BASE, byte);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
	while(I2CMasterBusy(I2C1_BASE));
}
