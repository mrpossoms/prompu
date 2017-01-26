#include "drv_LSM9DS0.h"
//#include "simpletools.h"

//#define LOG_LSM

int drv_LSM_init(i2c* bus)
{
  uint8_t byte = 0;

#ifdef LOG_LSM
  print("drv_LSM_init beginning...");
#endif

  if(i2c_read_reg_byte(bus, LSM_GRY_TMP, WHO_AM_I_G) != 0xd4)
    return -__COUNTER__;

  ctrl_reg1_g_t conf1 = {
    .datarate   = 3, // 760 Hz
    .bandwidth  = 3, // 100 (cutoff)
    .powered_up = 1,
    .x_on       = 1,
    .y_on       = 1,
    .z_on       = 1,
  };
  i2c_write_reg_byte(bus, LSM_GRY_TMP, CTRL_REG1_G, conf1.as_byte);

  ctrl_reg2_g_t conf2 = {
    .HPF_mode   = 0, // Normal filtering mode
    .HPF_cutoff = 4, // cutoff at 3.5 Hz
  };
  i2c_write_reg_byte(bus, LSM_GRY_TMP, CTRL_REG2_G, conf2.as_byte);

  // enable acc
  ctrl_reg1_xm_t xm_reg1 = {
    .x_on         = 1,
    .y_on         = 1,
    .z_on         = 1,
    .acc_datarate = 9,
  };
  i2c_write_reg_byte(bus, LSM_ACC_MAG, CTRL_REG1_XM, xm_reg1.as_byte);

  ctrl_reg5_xm_t xm_reg5 = {
    .mag_datarate = 5,
    .mag_res      = 0, // low resolution
    .temp_enabled = 1,
  };
  i2c_write_reg_byte(bus, LSM_ACC_MAG, CTRL_REG5_XM, xm_reg5.as_byte);

  ctrl_reg7_xm_t xm_reg7 = {
    .mag_mode      = 0,
    .mag_low_power = 0,
    .HPS_sel_acc   = 1, // use internal filter
    .HPF_mode_acc  = 0,
  };
  i2c_write_reg_byte(bus, LSM_ACC_MAG, CTRL_REG7_XM, xm_reg7.as_byte);

#ifdef LOG_LSM
  print("OK\n");
#endif
}

void drv_LSM_rot(i2c* bus, vec3_16i_t* rot)
{
  i2c_read_reg_range(
    bus,
    LSM_GRY_TMP,
    OUT_START_GYR,
    sizeof(vec3_16i_t),
    (uint8_t*)rot->buf
  );
}

void drv_LSM_acc(i2c* bus, vec3_16i_t* acc)
{
  i2c_read_reg_range(
    bus,
    LSM_ACC_MAG,
    OUT_START_ACC,
    sizeof(vec3_16i_t),
    (uint8_t*)acc
  );
}

void drv_LSM_mag(i2c* bus, vec3_16i_t* mag)
{
  i2c_read_reg_range(
    bus,
    LSM_ACC_MAG,
    OUT_START_MAG,
    sizeof(vec3_16i_t),
    (uint8_t*)mag
  );
}

uint16_t drv_LSM_temp(i2c* bus)
{
  uint16_t temp = 0;
  i2c_read_reg_range(
    bus,
    LSM_ACC_MAG,
    OUT_START_TMP,
    sizeof(temp),
    (uint8_t*)&temp
  );

  return temp;
}
