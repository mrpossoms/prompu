#define DEBUG
//#define PRINT_RAW
#define PRINT_QUAT
// #define THERMAL_STAT_COLLECTION

#include <math.h>
#include "boards.h"
#include "globals.h"
#include "i2c_util.h"
#include "uart.h"
#include "drv_LSM9DS0.h"
#include "MARG.h"

#define ACC_S 0.0000617f
#define GRY_S 0.01f
#define MAG_S 0.0005f

unsigned int LAST_CNT, CYCLES;

static uint16_t TEMP;
static vec3_16i_t ROT, ACC, MAG;

i2c_t I2C_IMU;

#define Q_HEX(off) *((int*)(Q + (off)))

void printfp(float f, int base)
{
	int i = f * base;
	int abs_i = abs(i);
	UARTprintf("%d.%d ", i / base, abs_i - (abs_i / base ));
}

void print_serial()
{
#ifdef PRINT_QUAT

	printfp(Q[0], 1000);	
	printfp(Q[1], 1000);	
	printfp(Q[2], 1000);	
	printfp(Q[3], 1000);
	UARTprintf("\n");

	//UARTprintf("%8x%8x%8x%8x\n", Q_HEX(0), Q_HEX(1), Q_HEX(2), Q_HEX(3));
#endif

#ifdef PRINT_RAW
	UARTprintf("ACC: %d, %d, %d\n", ACC.x, ACC.y, ACC.z);
	UARTprintf("gry: %d, %d, %d\n", ROT.x, ROT.y, ROT.z);
	UARTprintf("MAG: %d, %d, %d\n", MAG.x, MAG.y, MAG.z);
	UARTprintf("temp: %d\n", TEMP);
#endif
}

int main()
{
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
	i2c_init(&I2C_IMU, GPIO_PIN_6, GPIO_PIN_7, SYSCTL_PERIPH_GPIOA);
	uart_init(115200);
	
	FPUEnable();
	FPUStackingEnable();

#if defined(DEBUG) && !defined(THERMAL_STAT_COLLECTION)
	UARTprintf("Starting\n");
#endif


	I2C_SELECTED = &I2C_IMU;
	drv_LSM_init();

#ifdef TARGET_IS_PROPELLER
	LAST_CNT = CNT;
	const int WAIT = CLKFREQ / 10; // 10 HZ
	volatile unsigned int nextcnt = CNT + WAIT;
#endif

	while(1)
	{
#ifdef TARGET_IS_PROPELLER
		CYCLES = CNT - LAST_CNT;
		LAST_CNT = CNT;
#endif
		drv_LSM_vec3(LSM_ACC_MAG, LSM_START_ACC, &ACC);
		drv_LSM_vec3(LSM_GRY_TMP, LSM_START_GYR, &ROT);
		drv_LSM_vec3(LSM_ACC_MAG, LSM_START_MAG, &MAG);
		TEMP = drv_LSM_temp();

		MARG_tick(
			ROT.x, ROT.y, ROT.z,
			ACC.x, ACC.y, ACC.z,
			MAG.x, MAG.y, MAG.z,
			0.1
		);
#if defined(DEBUG)
		//print("Sec/tick : %f", dt);
		print_serial();
#endif

#ifdef TARGET_IS_PROPELLER
		nextcnt = waitcnt2(nextcnt, WAIT);
#endif

		ROM_SysCtlDelay(1000000);

	}

	return 0;
}
