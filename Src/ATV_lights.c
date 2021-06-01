/*
 * ATV_lights.c
 *
 *  Created on: May 20, 2021
 *      Author: rk-linux
 */

#include "ATV_lights.h"

void Set_Headlights(uint8_t value) {
	HAL_GPIO_WritePin(HEAD_LIGHTS_ON_OUT_GPIO_Port, HEAD_LIGHTS_ON_OUT_Pin, value);
}
void Set_Brake_Light(uint8_t value) {
	HAL_GPIO_WritePin(BRAKE_LIGHT_ON_OUT_GPIO_Port, BRAKE_LIGHT_ON_OUT_Pin, value);
}
void Set_Right_Blinker(uint8_t value) {
	HAL_GPIO_WritePin(RIGHT_BLINKER_ON_OUT_GPIO_Port, RIGHT_BLINKER_ON_OUT_Pin, value);
}
void Set_Left_Blinker(uint8_t value) {
	HAL_GPIO_WritePin(LEFT_BLINKER_ON_OUT_GPIO_Port, LEFT_BLINKER_ON_OUT_Pin, value);
}
