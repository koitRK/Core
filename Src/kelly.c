/*
 * kelly.c
 *
 *  Created on: May 20, 2021
 *      Author: rk-linux
 */


#include "kelly.h"

double kelly_min_throttle = 110;

void Set_Gear(uint8_t gear){
	HAL_GPIO_WritePin(ATV_GEAR_OUT_GPIO_Port, ATV_GEAR_OUT_Pin, gear);
}

void Set_Throttle(uint8_t throttle){
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, Map_To_Range(throttle, 0, 255, kelly_min_throttle, 480.0));
}
