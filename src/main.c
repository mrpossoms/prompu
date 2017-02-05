/*
	Welcome.c

	Welcome to SimpleIDE, the C programming environment that makes it easy to
	get started with the multi-core Propeller microcontroller!

	To run this program:

		- Is this the first Parallax software you've installed on your computer?
			If so, install USB driver's now: www.parallax.com/usbdrivers
		- Connect your Propeller board to your computer's USB.  Also connect power
			to the board if needed, and if it has a power switch, turn it on.
		- Make sure to select your COM port from the toolbar dropdown menu in the
			upper-right corner.  If you are unsure which COM port your board is
			connected to, disconnect it and click the dropdown to check the port
			list, then reconnect it and check again.
		- Click Program and select Run with Terminal (or click the Run with Terminal
			button).  The SimpleIDE Terminal should appear and display the "Hello!"
			message.

	 Next, check the Help menu for new online tutorials, software manual, and
	 reference material.
	 http://learn.parallax.com/propeller-c-tutorials
*/
#define DEBUG
// #define PRINT_RAW
#define PRINT_QUAT
// #define THERMAL_STAT_COLLECTION

#include <propeller.h>
#include "globals.h"
#include "i2c_util.h"
#include "drv_LSM9DS0.h"
#include "MARG.h"

#ifdef DEBUG
#include "simpletext.h"
#endif

#define ACC_S 0.0000617f
#define GRY_S 0.01f
#define MAG_S 0.0005f

unsigned int LAST_CNT, CYCLES;

static uint16_t TEMP;
static vec3_16i_t ROT, ACC, MAG;

int i2c_init()
{
	i2c_open(18, 17);
}

void print_serial()
{
#ifdef THERMAL_STAT_COLLECTION
	const char VEC_FMT[] = "%d,%d,%d,";

	print(VEC_FMT, ACC.x, ACC.y, ACC.z);
	print(VEC_FMT, ROT.x, ROT.y, ROT.z);
	print(VEC_FMT, MAG.x, MAG.y, MAG.z);
	print("%d\n", TEMP);
#endif

#ifdef PRINT_QUAT
	print(
		"Q = { %f, %f, %f, %f }\n",
		Q[0], Q[1], Q[2], Q[3]
	);
#endif

#ifdef PRINT_RAW
	print("ACC: %d, %d, %d\n", ACC.x, ACC.y, ACC.z);
	print("gry: %d, %d, %d\n", ROT.x, ROT.y, ROT.z);
	print("MAG: %d, %d, %d\n", MAG.x, MAG.y, MAG.z);
	print("temp: %d\n", TEMP);
#endif
}

void main()
{
	LAST_CNT = CNT;

#if defined(DEBUG) && !defined(THERMAL_STAT_COLLECTION)
	print("Starting\n");
#endif

	i2c_init();
	drv_LSM_init();

	const int WAIT = CLKFREQ / 10; // 10 HZ
	volatile unsigned int nextcnt = CNT + WAIT;

	while(1)
	{
		CYCLES = CNT - LAST_CNT;
		LAST_CNT = CNT;

		drv_LSM_vec3(LSM_ACC_MAG, LSM_START_ACC, &ACC);
		drv_LSM_vec3(LSM_GRY_TMP, LSM_START_GYR, &ROT);
		drv_LSM_vec3(LSM_ACC_MAG, LSM_START_MAG, &MAG);
		TEMP = drv_LSM_temp();

		float dt = MARG_tick(
			ROT.x / GRY_S, ROT.y / GRY_S, ROT.z / GRY_S,
			ACC.x / ACC_S, ACC.y / ACC_S, ACC.z / ACC_S,
			MAG.x / MAG_S, MAG.y / MAG_S, MAG.z / MAG_S
		);

#ifdef DEBUG
		print("Sec/tick : %f", dt);
		print_serial();
		nextcnt = waitcnt2(nextcnt, WAIT);
#endif
	}
}
