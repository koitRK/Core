/*
 * kelly.h
 *
 *  Created on: Apr 28, 2021
 *      Author: user
 */

#ifndef INC_KELLY_H_
#define INC_KELLY_H_

#include "main.h"
#include "tim.h"
#include "gpio.h"

#define GEAR_FORWARD 0
#define GEAR_REVERSE 1

void Set_Gear(uint8_t gear); // 0 forward / 1 reverse
void Set_Throttle(uint8_t throttle); // actual throttle voltage ranges 0.7V to 3.3V; throttle value ranges 0 to 255 and will be mapped so that 0 - 255 -> 0.7V - 3.3V (PWM)

#endif /* INC_KELLY_H_ */
