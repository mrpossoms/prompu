#ifndef __DRV_LSM9DS0
#define __DRV_LSM9DS0

#include "i2c_util.h"

//      _      _    _            __       ___
//     /_\  __| |__| |_ _ ___   / _|___  | _ \___ __ _ ___
//    / _ \/ _` / _` | '_(_-<_  > _|_ _| |   / -_) _` (_-<_
//   /_/ \_\__,_\__,_|_| /__(_) \_____|  |_|_\___\__, /__(_)
//                                               |___/
#define LSM_ACC_MAG   0x3C
#define LSM_GRY_TMP   0xD4

// Gyro register definitions
#define WHO_AM_I_G    0x0F
#define CTRL_REG1_G   0x20
#define CTRL_REG2_G   0x21
#define CTRL_REG4_G   0x23

#define OUT_START_GYR 0x28

#define CTRL_REG1_XM  0x20
#define CTRL_REG5_XM  0x24
#define CTRL_REG7_XM  0x26

#define OUT_START_ACC 0x28
#define OUT_START_MAG 0x08
#define OUT_START_TMP 0x05

//     ___ _       _   ___ _               _
//    / __| |_ _ _| | / __| |_ _ _ _  _ __| |_ ___
//   | (__|  _| '_| | \__ \  _| '_| || / _|  _(_-<
//    \___|\__|_| |_| |___/\__|_|  \_,_\__|\__/__/
//

typedef union {
	struct {
		uint8_t x_on       : 1;
		uint8_t y_on       : 1;
		uint8_t z_on       : 1;
		uint8_t powered_up : 1;
		uint8_t bandwidth  : 2;
		uint8_t datarate   : 2;
	};
	uint8_t as_byte;
} ctrl_reg1_g_t;

typedef union {
	struct {
		uint8_t HPF_cutoff   : 4;
		uint8_t HPF_mode     : 2;
		uint8_t NA           : 2;
	};
	uint8_t as_byte;
} ctrl_reg2_g_t;

typedef union {
	struct {
		uint8_t x_on            : 1;
		uint8_t y_on            : 1;
		uint8_t z_on            : 1;
		uint8_t blk_data_update : 1;
		uint8_t acc_datarate    : 4;
	};
	uint8_t as_byte;
} ctrl_reg1_xm_t;

typedef union {
	struct {
		uint8_t interrupts   : 2;
		uint8_t mag_datarate : 3;
		uint8_t mag_res      : 2;
		uint8_t temp_enabled : 1;
	};
	uint8_t as_byte;
} ctrl_reg5_xm_t;

typedef union {
	struct {
		uint8_t mag_mode     : 2;
		uint8_t mag_low_power: 1;
		uint8_t ZERO         : 2;
		uint8_t HPS_sel_acc  : 1;
		uint8_t HPF_mode_acc : 2;
	};
	uint8_t as_byte;
} ctrl_reg7_xm_t;

//    ___       _          ___ _               _
//   |   \ __ _| |_ __ _  / __| |_ _ _ _  _ __| |_ ___
//   | |) / _` |  _/ _` | \__ \  _| '_| || / _|  _(_-<
//   |___/\__,_|\__\__,_| |___/\__|_|  \_,_\__|\__/__/
//

typedef union {
	struct {
		int16_t x, y, z;
	};
	uint8_t buf[sizeof(int16_t) * 3];
} vec3_16i_t;

typedef union {
	struct {
		int8_t x, y, z;
	};
	uint8_t buf[sizeof(int8_t) * 3];
} vec3_8i_t;

int drv_LSM_init();

void     drv_LSM_rot(vec3_16i_t* rot);
void     drv_LSM_acc(vec3_16i_t* acc);
void     drv_LSM_mag(vec3_16i_t* mag);
uint16_t drv_LSM_temp();

#endif
