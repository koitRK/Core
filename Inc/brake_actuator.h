/*
 * brake_actuator.h
 *
 *  Created on: Apr 28, 2021
 *      Author: user
 */

#ifndef INC_BRAKE_ACTUATOR_H_
#define INC_BRAKE_ACTUATOR_H_

#include "main.h"
#include "gpio.h"
#include "brake_hall_sensor.h" // uses hall sensor to set brake actuator correct position(actuator doesn't have dedicated encoder)

#define BRAKE_SENSOR_OFF_VALUE 2850  // Brake sensor value when brakes are disengaged
#define BRAKE_SENSOR_ON_VALUE 3200   // Brake sensor value when brakes are fully engaged
#define BRAKE_ENGAGE 1
#define BRAKE_DISENGAGE 0

void Set_Brake(uint8_t target_brake_val); // moves actuator towards desired position; brake_val ranges 0 - 100, likely

#endif /* INC_BRAKE_ACTUATOR_H_ */
