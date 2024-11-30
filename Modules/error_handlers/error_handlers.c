/**
 ******************************************************************************
 * @file           : error_handlers.c
 * @author         : Jacek Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Handling all errors that is detected TODO
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------- */

#include <stm32h7xx_hal.h>
#include "error_handlers/error_handlers.h"
#include "leds/leds.h"
#include "communication/communication.h"

uint8_t error_count = 0;
Error_code current_error;

/* Functions ------------------------------------------------------------------- */

static void send_err_eth(Error_code error) {
	

	static uint8_t ID[2] = { 0 };
	static uint8_t send[16] = { 0 };
	static uint8_t hex[2] = { 0 };
	UART_encode(MAINBOARD_ERROR_ID, ID);
	for (uint8_t i = 0; i < 4; i++) {
		UART_encode(RxMsg[i], hex);
		send[2 * i] = hex[0];
		send[2 * i + 1] = hex[1];
	}
	Eth_sendData(ID, send);


}

static void send_err_bt() {
}

//static void send_err_can2() {
//}

/**
 ******************************************************************************
 * @details			:	Error handling - turn off leds and while loop
 ******************************************************************************
 */
void Error_Handler(Error_code error) {

	switch (error) {
	case CAN1Error_init ... CAN1Error_start:
		break;

	case CAN1Error_transferAddMessege ... CAN1Error_transferEnableTx:
		break;

	case CAN1Error_fifoGetMessege ... CAN1Error_fifoActivateNotification:
		break;

	case CAN2Error_init ... CAN2Error_start:
		break;

	case CAN2Error_transferAddMessege ... CAN2Error_transferEnableTx:
		break;

	case CAN2Error_fifoGetMessege ... CAN2Error_fifoActivateNotification:
		break;

	case TIM4Error_baseInit ... TIM4Error_configChannel:
		break;

	case TIM7Error_baseInit ... TIM7Error_configSync:
		break;

	case TIM16Error_baseInit:
		break;

	case SRCError_mainOscConfig ... SRCError_mainClockConfig:
		break;

	case HALError_CAN1 ... HALError_CAN2:
		break;

	case HALError_UART1PeriphClock ... HALError_UART1TX:
		break;
	}

	/// TODO: FInish Error handler!
	__disable_irq();

	Leds_init();
	Leds_turnOff(LED_ALL);
	Leds_turnOn(LED_4);
	while (1) {
	}
}
