/*
 * iBus.h
 *
 *  Created on: Apr 28, 2021
 *      Author: kristjan
 */

#ifndef INC_IBUS_H_
#define INC_IBUS_H_
#include <stdint.h>
#include "usart.h"

#define REC_SW_UP 1000
#define REC_SW_DOWN 2000
#define REC_SW_MID 1500

#define IBUS_HAS_2 (1 << 0)
#define IBUS_HAS_4 (1 << 1)
#define IBUS_HAS_MESSAGE (1 << 2)

UART_HandleTypeDef huart4;

extern volatile uint8_t ibus_rx_buffer[40];
extern volatile uint8_t ibus_rx_flags;
extern volatile uint8_t ibus_rx_xavier_buffer[20]; // XAVIER 11 bytes required, 20 ok?
extern volatile uint8_t ibus_rx_xavier_flags; // XAVIER


extern volatile uint8_t input_UART3_rxBuffer[9]; // from xavier, n-data bytes +1 stop byte (thr: 1B, str: 1B, brk: 1B, gear: 1B, h-light: 1B, R-light: 1B, L-light: 1B, confirm: 1B, +1 for stop: 1B)
extern volatile uint8_t output_UART3_rxBuffer[18]; // to xavier, n-data bytes (vel: 2B, enc: 2B, IMU: 2*3*2B=12B, E-stop: 1B, confirm: 1B)

uint8_t xavier_receive_failure_counter;


typedef enum
{
  ReceiverSwitchUp,
  ReceiverSwitchMiddle,
  ReceiverSwitchDown
} ReceiverSwitchState_e;

typedef struct
{
  // Joystick axes.
  uint16_t right_horizontal;
  uint16_t right_vertical;
  uint16_t left_vertical;
  uint16_t left_horizontal;

  // Receiver switches.
  ReceiverSwitchState_e switch_a;
  ReceiverSwitchState_e switch_b;
  ReceiverSwitchState_e switch_c;
  ReceiverSwitchState_e switch_d;
} ReceiverState_t;

typedef struct
{
	uint8_t throttle;
	uint8_t brake;
	uint8_t steering;
	uint8_t gear;
	uint8_t h_light;
	uint8_t r_light;
	uint8_t l_light;
} XavierCommand_t;

XavierCommand_t xavier_command;

//prototypes
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* uart);
ReceiverState_t DecodeReceiverState(uint8_t* buff);
ReceiverState_t HandleReceiverMessage();
void HandleXavierMessage();

void CheckUartInterruptStatus();
void CheckUartXavierInterruptStatus();

void Transmit_To_Xavier(uint8_t i);

#endif /* INC_IBUS_H_ */
