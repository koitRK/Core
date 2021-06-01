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


volatile uint8_t ibus_rx_buffer[40] = { 0 };
volatile uint8_t ibus_rx_flags = 0;

const uint8_t msg_len = 9; //input
int received = 0;
volatile uint8_t input_UART3_rxBuffer[9] = {0}; // XAVIER
volatile uint8_t ibus_rx_xavier_flags = 0; // XAVIER
volatile uint8_t ibus_rx_xavier_buffer[20] = {0}; // XAVIER 11 bytes required, 20 ok?

volatile uint8_t usable_data[9] = {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}; // XAVIER

volatile uint8_t output_UART3_rxBuffer[18] = {'V', 'V', 'E', 'E', 'I', 'M', 'U', 'I', 'M', 'U', 'I', 'M', 'U', 'I', 'M', 'U', 'S', 'C'}; // XAVIER


void handle_message(uint8_t read_buf[]){
	//memcpy(usable_data, read_buf, sizeof(read_buf));

    for(int i=0; i<sizeof(read_buf); i++)
    {
        usable_data[i] = read_buf[i];
    }
}


XavierCommand_t xavier_command = {.throttle=0, .steering=1, .brake=0, .gear=0};
uint8_t xavier_receive_failure_counter = 0;

void Transmit_To_Xavier(uint8_t i){ // TODO
	i += 48;
	uint8_t buffer[18] = {i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i};
	buffer[sizeof(buffer)-1] = 'C';
	//HAL_UART_Transmit(&huart3, output_UART3_rxBuffer, sizeof(output_UART3_rxBuffer), 100);
	HAL_UART_Transmit(&huart3, buffer, sizeof(buffer), 100);
}

void Transmit_Multiple_To_Xavier(uint8_t i){ // TODO
	i += 48;
	uint8_t buffer[18] = {i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i};
	buffer[sizeof(buffer)-1] = 'C';
	//HAL_UART_Transmit(&huart3, output_UART3_rxBuffer, sizeof(output_UART3_rxBuffer), 100);
	HAL_UART_Transmit(&huart3, buffer, sizeof(buffer), 100);
}

void Transmit_Char_To_Xavier(char c){ // TODO
	uint8_t buffer[18] = {c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c};
	buffer[sizeof(buffer)-1] = 'C';
	//HAL_UART_Transmit(&huart3, output_UART3_rxBuffer, sizeof(output_UART3_rxBuffer), 100);
	HAL_UART_Transmit(&huart3, buffer, sizeof(buffer), 100);
}

int Poll_Port() {

  auto count = HAL_UART_Receive_IT(&huart3,
									input_UART3_rxBuffer + received, 		  // Append to whatever we already have
									msg_len - received); // Only read enough to finish the current message.

  if(count != -1) {
    received += count;
    if(received == msg_len) {
      HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin); // Red LED if ERROR
      if (input_UART3_rxBuffer[sizeof(input_UART3_rxBuffer)-1] != 'C'){ // Read_buf is out of phase or has incorrect values
        received -= 1;

        return -1; // Buffer might not be empty
      }
      handle_message(input_UART3_rxBuffer);
      received = 0;
      return 1; // Buffer might not be empty
    }
  }
  return 0; // Buffer is empty
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
  if (uart == &huart3) // XAVIER COMMUNICATION NEW
  {
	  // iBus message format: 0x20 0x40 dd * 7 chk chk
	  // where the 0x20 0x40 are header bytes,
	  // dd are data bytes (7 of them, (thr: 1B, str: 1B, brk: 1B, gear: 1B, h-light: 1B, R-light: 1B, L-light: 1B))
	  // chk are checksum bytes.
	  // 11 bytes in total.

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
          // have header's second byte, queue the rest of the message (9 bytes).
		  ibus_rx_xavier_flags = IBUS_HAS_4;
		  HAL_UART_Receive_IT(uart, ibus_rx_xavier_buffer + 2, 9);
		}
	  }
	  else
	  {
        // the remainder of the message arrived, set the flag and don't listen to anything
		// else until main() has read it.
		// Raw message is in the ibus_rx_buffer variable.
		//ibus_rx_xavier_flags = IBUS_HAS_MESSAGE;  // Don't use this flag. Data might lag behind, if not processed fast enough.

		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

		for(int i=0; i<7; i++)
		{
			usable_data[i] = ibus_rx_xavier_buffer[i+2]; // Write data from buffer to usable data. Exclude 2 start bytes and 2 checksum bytes
		}

		ibus_rx_xavier_flags = 0; // Set flags to 0.

		for (uint8_t i = 0; i < 20; i++) // Fill buffer with zeros
		{
		  ibus_rx_xavier_buffer[i] = 0;
		}

		// Start receiving again.
		HAL_UART_Receive_IT(&huart3, ibus_rx_xavier_buffer, 1);

	  }
  }
  else if (1==0){ // XAVIER COMMUNICATION OLD??
	  //HAL_UART_Receive_IT(&huart3, input_UART3_rxBuffer, sizeof(input_UART3_rxBuffer));

	  while (Poll_Port() != 0){
	  }

	  output_UART3_rxBuffer[1] = usable_data[1];
	  output_UART3_rxBuffer[2] = 'Y';
	  output_UART3_rxBuffer[3] = input_UART3_rxBuffer[3];

	  HAL_UART_Transmit(&huart3, output_UART3_rxBuffer, sizeof(output_UART3_rxBuffer), 100);

	  //Transmit_Char_To_Xavier(input_UART3_rxBuffer[0]);

	  // Check if received message last data byte (confirmation byte) is correct: [-2] == 'C'
	  if (input_UART3_rxBuffer[sizeof(input_UART3_rxBuffer)-2] == 'C'){
		  // Data is OK, overwrite xavier_command
		  xavier_receive_failure_counter = 0; // Reset failure counter

		  xavier_command.throttle = input_UART3_rxBuffer[0];
		  xavier_command.steering = input_UART3_rxBuffer[1];
		  xavier_command.brake = input_UART3_rxBuffer[2];
		  xavier_command.gear = input_UART3_rxBuffer[3];
		  xavier_command.h_light = input_UART3_rxBuffer[4];
		  xavier_command.r_light = input_UART3_rxBuffer[5];
		  xavier_command.l_light = input_UART3_rxBuffer[6];

		  //HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
		  //HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 1); // Green LED if OK
	  }
	  else{
		  // Data is not OK
		  xavier_receive_failure_counter += 1;

		  if (xavier_receive_failure_counter >= 5){
			  // 5 consecutive failures, overwrite xavier_command to stopping state
			  xavier_command.throttle = THROTTLE_OFF;
			  xavier_command.steering = STEER_OFF;
			  xavier_command.brake = BRAKE_ENGAGE;
			  xavier_command.gear = GEAR_FORWARD;
			  xavier_command.r_light = LIGHT_ON;
			  xavier_command.l_light = LIGHT_ON;
		  }
		  // Else keep previous commads
		  //HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1); // Red LED if ERROR
		  //HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 0);
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

void HandleXavierMessage()
{
  // Decode the message.
  handle_message(&ibus_rx_xavier_buffer);

  // Reset the RX ISR flags.
  ibus_rx_xavier_flags = 0;

  for (uint8_t i = 0; i < 20; i++)
  {
	// Zero fill the buffer.
    ibus_rx_xavier_buffer[i] = 0;
  }

  // Start receiving again.
  HAL_UART_Receive_IT(&huart3, ibus_rx_xavier_buffer, 1);
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

void CheckUartXavierInterruptStatus()
{
  uint8_t uart_status = HAL_UART_GetState(&huart3);

  if (uart_status == 32)
  {
	// Weird ISR error. Reset state and try again.
	ibus_rx_xavier_flags = 0;
	HAL_UART_Receive_IT(&huart3, ibus_rx_xavier_buffer, 1);
  }
}
