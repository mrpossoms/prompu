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

#include "i2c_util.h"
#include "drv_LSM9DS0.h"
#include "MARG.h"

#ifdef DEBUG
#include "simpletext.h"
#endif

#define SEN_I2C_PTR (&g_sensor_i2c)


int i2c_init()
{
	i2c_open(18, 17);
}

void main()
{
#ifdef DEBUG
	print("Starting\n");
#endif
	i2c_init();
#ifdef DEBUG
	print("I2C up\n");
#endif
	drv_LSM_init();
#ifdef DEBUG
	print("I2C drivers up\n");
#endif
	vec3_16i_t rot, acc, mag;
	while(1)
	{
		drv_LSM_vec3(LSM_ACC_MAG, LSM_START_ACC, &acc);
		drv_LSM_vec3(LSM_GRY_TMP, LSM_START_GYR, &rot);
		drv_LSM_vec3(LSM_ACC_MAG, LSM_START_MAG, &mag);

		MARG_tick(
			rot.x, rot.y, rot.z,
			acc.x, acc.y, acc.z,
			mag.x, mag.y, mag.z
		);

#ifdef DEBUG
		print(
			"Q = { %f, %f, %f, %f }\n",
			Q[0], Q[1], Q[2], Q[3]
		);

		print("acc: %d, %d, %d\n", acc.x, acc.y, acc.z);
		print("gry: %d, %d, %d\n", rot.x, rot.y, rot.z);
		print("mag: %d, %d, %d\n", mag.x, mag.y, mag.z);
		print("temp: %d\n", drv_LSM_temp());

		sleep(1);
#endif
	}
}
