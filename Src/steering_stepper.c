/*
 * steering_stepper.c
 *
 *  Created on: 29. apr 2021
 *      Author: user
 */

#include "steering_stepper.h"

void Steering_Stepper_Move(uint8_t dir, uint8_t enable){
	HAL_GPIO_WritePin(STEERING_STEPPER_DIRECTION_OUT_GPIO_Port, STEERING_STEPPER_DIRECTION_OUT_Pin, dir);
	HAL_GPIO_WritePin(STEERING_STEPPER_ENABLE_OUT_GPIO_Port, STEERING_STEPPER_ENABLE_OUT_Pin, 0);

	const uint16_t encoder_val = Get_Steering_Encoder_Value();

	if(enable)
	{
		if(dir && encoder_val < STEERING_ENCODER_LEFT_VALUE)
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 5);
		}
		else if(dir == 0 && encoder_val > STEERING_ENCODER_RIGHT_VALUE)
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 5);
		}else
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); // perhaps should make ENABLE pin high
		}
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	}
};
