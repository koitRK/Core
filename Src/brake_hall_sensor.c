/*
 * brake_hall_sensor.c
 *
 *  Created on: Apr 29, 2021
 *      Author: rk-linux
 */

#include "brake_hall_sensor.h"

uint16_t Get_Brake_Sensor_Value()
{
  ADC_ChannelConfTypeDef sConfig;

  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 10);

  return HAL_ADC_GetValue(&hadc1);
}
