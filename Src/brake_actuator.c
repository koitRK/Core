/*
 * brake_actuator.c
 *
 *  Created on: Apr 29, 2021
 *      Author: user
 */

#include "brake_actuator.h"

void Set_Brake(const uint8_t target_brake_val){ // 1 - brake / 0 - release brake

	const uint16_t brake_sensor_value = Get_Brake_Sensor_Value();

	HAL_GPIO_WritePin(BRAKE_ACTUATOR_DIRECTION_OUT_GPIO_Port, BRAKE_ACTUATOR_DIRECTION_OUT_Pin, target_brake_val); // set actuator direction

	if(target_brake_val){ // brake
		if(brake_sensor_value < BRAKE_SENSOR_ON_VALUE) // brake only if actuator can push more
			HAL_GPIO_WritePin(BRAKE_ACTUATOR_ENABLE_OUT_GPIO_Port, BRAKE_ACTUATOR_ENABLE_OUT_Pin, 1);
		else // don't brake if brake is already ON
			HAL_GPIO_WritePin(BRAKE_ACTUATOR_ENABLE_OUT_GPIO_Port, BRAKE_ACTUATOR_ENABLE_OUT_Pin, 0);

	}else{ // release brake
		if(brake_sensor_value > BRAKE_SENSOR_OFF_VALUE) // release brake only if actuator can contract more
			HAL_GPIO_WritePin(BRAKE_ACTUATOR_ENABLE_OUT_GPIO_Port, BRAKE_ACTUATOR_ENABLE_OUT_Pin, 1);
		else // don't release brake if brake is already OFF
			HAL_GPIO_WritePin(BRAKE_ACTUATOR_ENABLE_OUT_GPIO_Port, BRAKE_ACTUATOR_ENABLE_OUT_Pin, 0);
	}
};
