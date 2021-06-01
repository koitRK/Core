/*
 * imu.h
 *
 *  Created on: May 27, 2021
 *      Author: rk-linux
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

typedef struct
{
  uint16_t acc[3];
  uint16_t gyro[3];

} imu_t;

#endif /* INC_IMU_H_ */
