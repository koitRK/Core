/*
 * control_signal.h
 *
 *  Created on: May 12, 2021
 *      Author: rk-linux
 */

#ifndef INC_CONTROL_SIGNAL_H_
#define INC_CONTROL_SIGNAL_H_

typedef struct
{
  uint8_t throttle;
  uint8_t brake;
  uint8_t steering;
  uint8_t gear;
  uint8_t handbrake;
  uint8_t steering_lock;
  uint8_t xavier_override;
  uint8_t h_light;
  uint8_t r_light;
  uint8_t l_light;
} ControlSignal_t;

#endif /* INC_CONTROL_SIGNAL_H_ */
