/*
 * utils.c
 *
 *  Created on: May 12, 2021
 *      Author: rk-linux
 */

#include "utils.h"

uint32_t Map_To_Range_int(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double Map_To_Range(double x, double in_min, double in_max, double out_min, double out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double Clamp(double d, double min, double max) {
  const double t = d < min ? min : d;
  return t > max ? max : t;
}
