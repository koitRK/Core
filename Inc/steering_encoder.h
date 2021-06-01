/*
 * steering_encoder.h
 *
 *  Created on: Apr 28, 2021
 *      Author: user
 */

#ifndef INC_STEERING_ENCODER_H_
#define INC_STEERING_ENCODER_H_

#include "main.h" // gets defined pins
#include "gpio.h" // gets functions for needed peripherals

#define STEERING_ENCODER_LEFT_VALUE 3400	// Encoder value when vehicle is steering left. On our ATV left > right
#define STEERING_ENCODER_RIGHT_VALUE 1800	// Encoder value when vehicle is steering right

uint16_t Get_Steering_Encoder_Value(); // returns 12-bit absolute encoder position

#endif /* INC_STEERING_ENCODER_H_ */
