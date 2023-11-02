#include "accel.h"

static int16_t data_raw_acceleration[3];
static float acceleration_mg[3];
static uint8_t whoamI;
static stmdev_ctx_t dev_ctx;


static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len)
{
  /* Write multiple command */
  reg |= 0x80;
  HAL_I2C_Mem_Write(handle, LIS331DLH_I2C_ADD_L, reg,
                    I2C_MEMADD_SIZE_8BIT, (uint8_t*) bufp, len, 1000);
  return 0;
}

static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len)
{
  /* Read multiple command */
  reg |= 0x80;
  HAL_I2C_Mem_Read(handle, LIS331DLH_I2C_ADD_L, reg,
                   I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
  return 0;
}


void accel_init(I2C_HandleTypeDef * i2c_device){
  /* Initialize mems driver interface */

  dev_ctx.write_reg = platform_write;
  dev_ctx.read_reg = platform_read;
  dev_ctx.handle = i2c_device;
  /* Initialize platform specific hardware */
  HAL_Delay(5);
  /* Check device ID */
  lis331dlh_device_id_get(&dev_ctx, &whoamI);

  if (whoamI != LIS331DLH_ID) {
	while (1) {
	  /* manage here device not found */
	}
  }

  /* Enable Block Data Update */
  lis331dlh_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
  /* Set full scale */
  lis331dlh_full_scale_set(&dev_ctx, LIS331DLH_2g);
  /* Configure filtering chain */
  /* Accelerometer - High Pass / Slope path */
  lis331dlh_hp_path_set(&dev_ctx, LIS331DLH_HP_DISABLE);
  //lis331dlh_hp_path_set(&dev_ctx, LIS331DLH_HP_ON_OUT);
  //lis331dlh_hp_reset_get(&dev_ctx);
  /* Set Output Data Rate */
  lis331dlh_data_rate_set(&dev_ctx, LIS331DLH_ODR_5Hz);

}

int accel_get_data(float * data)
{
  /* Read samples in polling mode (no int) */
  /* Read output only if new value is available */
  lis331dlh_reg_t reg;
  lis331dlh_status_reg_get(&dev_ctx, &reg.status_reg);
  if(!reg.status_reg.zyxda)
	  return -1;

  /* Read acceleration data */
  memset(data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
  lis331dlh_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
  acceleration_mg[0] =
		  lis331dlh_from_fs2_to_mg(data_raw_acceleration[0]);
  acceleration_mg[1] =
	lis331dlh_from_fs2_to_mg(data_raw_acceleration[1]);
  acceleration_mg[2] =
	lis331dlh_from_fs2_to_mg(data_raw_acceleration[2]);

  for(size_t i = 0; i < 3; i++)
	  data[i] = acceleration_mg[i];
  return 0;
}

