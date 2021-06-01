/*
 * speedometer.c
 *
 *  Created on: May 20, 2021
 *      Author: rk-linux
 */

#include "speedometer.h"

uint32_t current_speed_encoder_signal = 0;
uint32_t previous_speed_encoder_signal = 0;

uint16_t Get_Speed(){
	uint32_t signal_time_difference = current_speed_encoder_signal - previous_speed_encoder_signal;
	uint32_t current_signal_time_elapsed = HAL_GetTick() - current_speed_encoder_signal;
	uint16_t velocity = 0;
	if(current_signal_time_elapsed < 2000){
		if(signal_time_difference > current_signal_time_elapsed){
			velocity = wheel_distance_between_signals / signal_time_difference; // should check if time == 0
		}else{
			velocity = wheel_distance_between_signals / current_signal_time_elapsed;
		}
	}
	return velocity;
}
