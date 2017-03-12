#include "boards.h"
#include "uart.h"

void uart_init(unsigned int baud)
{

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	
	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
	ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	ROM_UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	UARTStdioConfig(0, baud, 16000000);
}

void uart_write(uint8_t* buf, unsigned int len)
{
	for(int i = 0; i < len; ++i)
	{
		ROM_UARTCharPut(UART0_BASE, buf[i]);
	}
}

void uart_read(uint8_t* buf, unsigned int len)
{
	for(int i = 0; i < len; ++i)
	{
		buf[i] = ROM_UARTCharGet(UART0_BASE);
	}
}
