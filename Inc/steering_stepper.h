/*
 * steering_stepper.h
 *
 *  Created on: Apr 28, 2021
 *      Author: user
 */

#ifndef INC_STEERING_STEPPER_H_
#define INC_STEERING_STEPPER_H_

#include "main.h"
#include "gpio.h"
#include "tim.h" // gets PWM peripherals
#include "steering_encoder.h"

#define STEER_DIRECTION_RIGHT 1
#define STEER_DIRECTION_LEFT 0
#define STEER_ENABLED 1
#define STEER_DISABLED 0

void Steering_Stepper_Move(uint8_t dir, uint8_t enable); // dir 0 or 1; enable 0 or 1

#endif /* INC_STEERING_STEPPER_H_ */
