/*
 * speedometer.h
 *
 *  Created on: May 20, 2021
 *      Author: rk-linux
 */

#ifndef INC_SPEEDOMETER_H_
#define INC_SPEEDOMETER_H_

#include "main.h"

#define wheel_distance_between_signals 350000 // um

uint32_t current_speed_encoder_signal;
uint32_t previous_speed_encoder_signal;

uint16_t Get_Speed();

#endif /* INC_SPEEDOMETER_H_ */
