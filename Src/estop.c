/*
 * estop.c
 *
 *  Created on: Apr 29, 2021
 *      Author: rk-linux
 */

#include "estop.h"


uint8_t Read_Estop(){
	return HAL_GPIO_ReadPin(ESTOP_IN_GPIO_Port, ESTOP_IN_Pin);
}
