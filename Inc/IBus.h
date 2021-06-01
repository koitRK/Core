/*
 * IBus.h
 *
 *  Created on: Apr 28, 2021
 *      Author: user
 */

#ifndef INC_IBUS_H_
#define INC_IBUS_H_

#include "main.h"
#include "usart.h"

void Init_IBus();
uint8_t IBus_Read_Channel(uint8_t channel_number); // might look something like this, "copied" from arduino IBusBM implementation

#endif /* INC_IBUS_H_ */
