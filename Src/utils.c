/*
 * utils.c
 *
 *  Created on: May 12, 2021
 *      Author: rk-linux
 */

#include "utils.h"


double Map_To_Range(double x, double in_min, double in_max, double out_min, double out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double Clamp(double d, double min, double max) {
  const double t = d < min ? min : d;
  return t > max ? max : t;
}
