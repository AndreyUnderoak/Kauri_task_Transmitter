#ifndef INC_ACCEL_H_
#define INC_ACCEL_H_

#include "stm32f4xx_hal.h"
#include "lis331dlh_reg.h"

/**
  * @brief  Initialization of Liss sensor
  *
  * @param  i2c_device  i2c handle in I2C_HandleTypeDef(ptr)
  */
void accel_init(I2C_HandleTypeDef* i2c_device);

/**
  * @brief  Getter for data in float
  *
  * @param  data  to data buffer in  float[3](ptr)
  * @retval       interface status 0->ready to get data
  */
int accel_get_data(uint8_t* data);


#endif /* INC_ACCEL_H_ */
