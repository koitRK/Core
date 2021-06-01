/*
 * ATV_lights.h
 *
 *  Created on: Apr 28, 2021
 *      Author: user
 */

#ifndef INC_ATV_LIGHTS_H_
#define INC_ATV_LIGHTS_H_

#include "main.h"
#include "gpio.h"

#define LIGHT_ON 0
#define LIGHT_OFF 1

void Set_Headlights(uint8_t value);
void Set_Brake_Light(uint8_t value);
void Set_Left_Blinker(uint8_t value);
void Set_Right_Blinker(uint8_t value);

#endif /* INC_ATV_LIGHTS_H_ */
