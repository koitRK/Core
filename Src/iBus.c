/*
 * iBus.c
 *
 *  Created on: Apr 28, 2021
 *      Created by: Erki, modified by: kristjan
 */

#include "iBus.h"
#include "usart.h"
#include "gpio.h"
#include <stdint.h>
#include "ATV_lights.h"
#include "brake_actuator.h"
#include "kelly.h"


volatile uint8_t ibus_rx_buffer[40] = {0};
volatile uint8_t ibus_rx_flags = 0;
volatile uint8_t ibus_rx_xavier_buffer[20] = {0};
volatile uint8_t ibus_rx_xavier_flags = 0;


volatile uint8_t latest_full_xavier_message[10] = {0};

//volatile uint8_t output_UART3_rxBuffer[18] = {'V', 'V', 'E', 'E', 'I', 'M', 'U', 'I', 'M', 'U', 'I', 'M', 'U', 'I', 'M', 'U', 'S', 'C'};

XavierCommand_t xavier_command = {.throttle=0, .steering=1, .brake=0, .gear=0};
uint8_t xavier_receive_failure_counter = 0;

uint8_t checksum(unsigned char *ptr, int sz) {
  uint8_t chk = 0;
  while (sz-- != 0){
    chk -= *ptr++;
  }
  return chk;
}

void Transmit_To_Xavier(MessageToXavier_t message){ // Transmit message to Xavier
	uint8_t buffer[18] = {0};
	buffer[0] = (message.velocity & 0xff);
	buffer[1] = (message.velocity >> 8);
	buffer[2] = (message.steering_encoder & 0xff);
	buffer[3] = (message.steering_encoder >> 8);
	buffer[4] = (message.imu.acc[0] & 0xff);
	buffer[5] = (message.imu.acc[0] >> 8);
	buffer[6] = (message.imu.acc[1] & 0xff);
	buffer[7] = (message.imu.acc[1] >> 8);
	buffer[8] = (message.imu.acc[2] & 0xff);
	buffer[9] = (message.imu.acc[2] >> 8);
	buffer[10] = (message.imu.gyro[0] & 0xff);
	buffer[11] = (message.imu.gyro[0] >> 8);
	buffer[12] = (message.imu.gyro[1] & 0xff);
	buffer[13] = (message.imu.gyro[1] >> 8);
	buffer[14] = (message.imu.gyro[2] & 0xff);
	buffer[15] = (message.imu.gyro[2] >> 8);
	buffer[16] = (message.estop);
	buffer[17] = 'C';
	HAL_UART_Transmit(&huart3, buffer, sizeof(buffer), 100);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef* uart)
{
  // In this example, we use huart2 for iBus. Since this complete callback function
  // is shared between the all of the huarts, we first check which huart is invoking it.
  if (uart == &huart4)
  {
	  // iBus message format: 0x20 0x40 dd * 28 chk chk
	  // where the 0x20 0x40 are header bytes,
	  // dd are data bytes (28 of them, for 14 channels; each channel is 2 bytes)
	  // chk are checksum bytes.
	  // 32 bytes in total.

	  // Blog posts about reversing this protocol: https://basejunction.wordpress.com/2015/08/23/en-flysky-i6-14-channels-part1/
	  // http://blog.dsp.id.au/posts/2017/10/22/flysky-ibus-protocol/
	  if (!ibus_rx_flags)
	  {
		if (ibus_rx_buffer[0] == 0x20)
		{
		  // have header's first byte, queue the second byte.
		  ibus_rx_flags = IBUS_HAS_2;
		  HAL_UART_Receive_IT(uart, ibus_rx_buffer + 1, 1);
		}
		else
		{
		  HAL_UART_Receive_IT(uart, ibus_rx_buffer, 1);
		}
	  }
	  else if (ibus_rx_flags == IBUS_HAS_2)
	  {
		if (ibus_rx_buffer[1] != 0x40)
		{
          // header's second byte doesn't match, reset and go back to stage 1.
		  ibus_rx_flags = 0;
		  HAL_UART_Receive_IT(uart, ibus_rx_buffer, 1);
		}
		else
		{
          // have header's second byte, queue the rest of the message (30 bytes).
		  ibus_rx_flags = IBUS_HAS_4;
		  HAL_UART_Receive_IT(uart, ibus_rx_buffer + 2, 30);
		}
	  }
	  else
	  {
        // the remainder of the message arrived, set the flag and don't listen to anything
		// else until main() has read it.
		// Raw message is in the ibus_rx_buffer variable.
		ibus_rx_flags = IBUS_HAS_MESSAGE;
	  }
  }
  if (uart == &huart3) // XAVIER COMMUNICATION
  {
	  // iBus message format: 0x20 0x40 dd * 7 chk
	  // where the 0x20 0x40 are header bytes,
	  // dd are data bytes (7 of them, (thr: 1B, str: 1B, brk: 1B, gear: 1B, h-light: 1B, R-light: 1B, L-light: 1B))
	  // chk is checksum byte.
	  // 10 bytes in total.

	  if (!ibus_rx_xavier_flags)
	  {
		if (ibus_rx_xavier_buffer[0] == 0x20)
		{
		  // have header's first byte, queue the second byte.
		  ibus_rx_xavier_flags = IBUS_HAS_2;
		  HAL_UART_Receive_IT(uart, ibus_rx_xavier_buffer + 1, 1);
		}
		else
		{
		  HAL_UART_Receive_IT(uart, ibus_rx_xavier_buffer, 1);
		}
	  }
	  else if (ibus_rx_xavier_flags == IBUS_HAS_2)
	  {
		if (ibus_rx_xavier_buffer[1] != 0x40)
		{
          // header's second byte doesn't match, reset and go back to stage 1.
		  ibus_rx_xavier_flags = 0;
		  HAL_UART_Receive_IT(uart, ibus_rx_xavier_buffer, 1);
		}
		else
		{
          // have header's second byte, queue the rest of the message (8 bytes).
		  ibus_rx_xavier_flags = IBUS_HAS_4;
		  HAL_UART_Receive_IT(uart, ibus_rx_xavier_buffer + 2, 8);
		}
	  }
	  else
	  {
        // the remainder of the message arrived, write it to usable_data and start receiving next message.

		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

		for(int i=0; i<sizeof(ibus_rx_xavier_buffer); i++)
		{
			latest_full_xavier_message[i] = ibus_rx_xavier_buffer[i]; // Save latest full message
		}

		Handle_Xavier_Message(latest_full_xavier_message); // Check and handle message

		ibus_rx_xavier_flags = 0; // Set flags to 0.

		for (uint8_t i = 0; i < 20; i++) // Fill buffer with zeros
		{
		  ibus_rx_xavier_buffer[i] = 0;
		}

		// Start receiving again.
		HAL_UART_Receive_IT(&huart3, ibus_rx_xavier_buffer, 1);

	  }
  }
}


#define JOIN_TO_WORD(arr) ((*(arr)) | (*((arr) + 1) << 8))

ReceiverState_t DecodeReceiverState(uint8_t* buff)
{
  ReceiverState_t rec = { 0 };

  uint16_t ch1 = JOIN_TO_WORD(buff + 2);
  uint16_t ch2 = JOIN_TO_WORD(buff + 4);
  uint16_t ch3 = JOIN_TO_WORD(buff + 6);
  uint16_t ch4 = JOIN_TO_WORD(buff + 8);
  uint16_t ch5 = JOIN_TO_WORD(buff + 10);
  uint16_t ch6 = JOIN_TO_WORD(buff + 12);
  uint16_t ch7 = JOIN_TO_WORD(buff + 14);
  uint16_t ch8 = JOIN_TO_WORD(buff + 16);

  rec.right_horizontal = ch1;
  rec.right_vertical = ch3;
  rec.left_vertical = ch2;
  rec.left_horizontal = ch4;

  if (ch5 == REC_SW_UP)
    rec.switch_a = ReceiverSwitchUp;
  else
    rec.switch_a = ReceiverSwitchDown;

  if (ch8 == REC_SW_UP)
    rec.switch_b = ReceiverSwitchUp;
  else if (ch8 > 1200 && ch8 <= 1700)
    rec.switch_b = ReceiverSwitchMiddle;
  else
    rec.switch_b = ReceiverSwitchDown;

  if (ch7 > 0 && ch7 <= 1200)
    rec.switch_c = ReceiverSwitchUp;
  else if (ch7 > 1200 && ch7 <= 1700)
    rec.switch_c = ReceiverSwitchMiddle;
  else
    rec.switch_c = ReceiverSwitchDown;

  if (ch6 == REC_SW_UP)
    rec.switch_d = ReceiverSwitchUp;
  else
    rec.switch_d = ReceiverSwitchDown;

  return rec;
}

#undef JOIN_TO_WORD

ReceiverState_t HandleReceiverMessage()
{
  // Decode the message.
  ReceiverState_t rec = DecodeReceiverState(ibus_rx_buffer);

  // Reset the RX ISR flags.
  ibus_rx_flags = 0;

  for (uint8_t i = 0; i < 40; i++)
  {
	// Zero fill the buffer.
    ibus_rx_buffer[i] = 0;
  }

  // Start receiving again.
  HAL_UART_Receive_IT(&huart4, ibus_rx_buffer, 1);

  return rec;
}

void Handle_Xavier_Message(uint8_t full_xavier_message[])
{

	uint8_t chk = checksum(full_xavier_message, 10);

	if (chk != 0){ // Checksum does not match. Do not overwrite xavier_command
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1); // Show RED led
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 0);

		xavier_receive_failure_counter += 1; // Count consecutive failures

		if (xavier_receive_failure_counter >= 5)
		{
			// 5 consecutive failures, overwrite xavier_command to stopping state
			xavier_command.throttle = THROTTLE_OFF;
			xavier_command.steering = STEER_OFF;
			xavier_command.brake = BRAKE_ENGAGE;
			xavier_command.gear = GEAR_FORWARD;
			xavier_command.r_light = LIGHT_ON;
			xavier_command.l_light = LIGHT_ON;
		}
		// Else keep previous commands
	}
	else{ // Checksum matches. Data OK. Overwrite xavier_command
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0); // Show GREEN led
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 1);

		xavier_receive_failure_counter = 0; // Reset failure counter

		xavier_command.throttle = full_xavier_message[2]; // Data bytes start from index 2
		xavier_command.steering = full_xavier_message[3];
		xavier_command.brake = full_xavier_message[4];
		xavier_command.gear = full_xavier_message[5];
		xavier_command.h_light = full_xavier_message[6];
		xavier_command.r_light = full_xavier_message[7];
		xavier_command.l_light = full_xavier_message[8];
	}
}

void CheckUartInterruptStatus()
{
  uint8_t uart_status = HAL_UART_GetState(&huart4);

  if (uart_status == 32)
  {
	// Weird ISR error. Reset state and try again.
	ibus_rx_flags = 0;
	HAL_UART_Receive_IT(&huart4, ibus_rx_buffer, 1);
  }
}

