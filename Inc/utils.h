/*
 * utils.h
 *
 *  Created on: May 12, 2021
 *      Author: rk-linux
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <stdint.h> // for uintxx_t

uint32_t Map_To_Range_int(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
double Map_To_Range(double x, double in_min, double in_max, double out_min, double out_max);
double Clamp(double d, double min, double max);

#endif /* INC_UTILS_H_ */
