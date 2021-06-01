#include "steering_encoder.h"

uint16_t Get_Steering_Encoder_Value() {
	int16_t i;
	char Resolution = 12;
	uint32_t bitstart = 0x0800;
	uint16_t encoder_reading = 0;

	HAL_GPIO_WritePin(STEERING_ENCODER_CS_OUT_GPIO_Port, STEERING_ENCODER_CS_OUT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STEERING_ENCODER_CLK_OUT_GPIO_Port, STEERING_ENCODER_CLK_OUT_Pin, GPIO_PIN_RESET);

	for (i = (Resolution - 1); i >= 0; i--) {
		HAL_GPIO_WritePin(STEERING_ENCODER_CLK_OUT_GPIO_Port,STEERING_ENCODER_CLK_OUT_Pin, GPIO_PIN_SET);

		if (HAL_GPIO_ReadPin(STEERING_ENCODER_DO_IN_GPIO_Port, STEERING_ENCODER_DO_IN_Pin))
			encoder_reading |= bitstart;
		{
			HAL_GPIO_WritePin(STEERING_ENCODER_CLK_OUT_GPIO_Port,STEERING_ENCODER_CLK_OUT_Pin, GPIO_PIN_RESET);
			bitstart = bitstart >> 1;
		}
		if (i == 0) {
			HAL_GPIO_WritePin(STEERING_ENCODER_CLK_OUT_GPIO_Port,STEERING_ENCODER_CLK_OUT_Pin, GPIO_PIN_SET);
			if (HAL_GPIO_ReadPin(STEERING_ENCODER_DO_IN_GPIO_Port, STEERING_ENCODER_DO_IN_Pin))
				encoder_reading |= bitstart;

		}

	}
	HAL_GPIO_WritePin(STEERING_ENCODER_CS_OUT_GPIO_Port, STEERING_ENCODER_CS_OUT_Pin, GPIO_PIN_SET);

	return encoder_reading;

}
