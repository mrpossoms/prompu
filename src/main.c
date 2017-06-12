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
	if(isnan(f))
	{
		UARTprintf("NaN ");
		return;
	}

	int i = f * base;
	int whole = ((int)f) * base; 
	UARTprintf("%d.%d ", i / base, abs(i - whole));
}

void print_serial()
{
#ifdef PRINT_QUAT

	printfp(Q[1], 10000);	
	printfp(Q[2], 10000);	
	printfp(Q[3], 10000);	
	printfp(Q[0], 10000);
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
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_8MHZ | SYSCTL_OSC_MAIN);
	i2c_init(&I2C_IMU, GPIO_PIN_6, GPIO_PIN_7, SYSCTL_PERIPH_GPIOA);
	uart_init(115200);
	
	FPUEnable();
	FPUStackingEnable();

#if defined(DEBUG) && !defined(THERMAL_STAT_COLLECTION)
	UARTprintf("Starting\n");
#endif

	I2C_SELECTED = &I2C_IMU;
	drv_LSM_init();
	MARG_reset();

	const float dt = 0.01;
	uint32_t delay = SysCtlClockGet() * dt;

	while(1)
	//for(int i = 5; i--;)
	{
		drv_LSM_vec3(LSM_ACC_MAG, LSM_START_ACC, &ACC);
		drv_LSM_vec3(LSM_GRY_TMP, LSM_START_GYR, &ROT);
		drv_LSM_vec3(LSM_ACC_MAG, LSM_START_MAG, &MAG);
		TEMP = drv_LSM_temp();

		const float rot_scl = (245.f / 32768.f) * (3.14159f / 180.f);
		const float acc_scl = 2.f / 32768.f;
		MARG_tick(
			ROT.x * rot_scl, ROT.y * rot_scl, ROT.z * rot_scl,
			ACC.x * acc_scl, ACC.y * acc_scl, ACC.z * acc_scl,
			MAG.x, MAG.y, MAG.z,
			dt
		);
#if defined(DEBUG)
		//print("Sec/tick : %f", dt);
		print_serial();
#endif

		ROM_SysCtlDelay(delay);
		//break;
	}

	return 0;
}
