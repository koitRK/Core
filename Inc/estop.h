/*
 * estop.h
 *
 *  Created on: Apr 29, 2021
 *      Author: rk-linux
 */

#ifndef INC_ESTOP_H_
#define INC_ESTOP_H_

#include "main.h"
#include "gpio.h"

#define ESTOP_ACTIVATED 0	 // E-stop value when e-stop is activated

uint8_t Read_Estop();

#endif /* INC_ESTOP_H_ */
