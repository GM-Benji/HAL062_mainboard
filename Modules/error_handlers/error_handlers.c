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

#include "timers/timers.h"
#include "can/can.h"
#include <stm32h7xx_hal.h>

uint8_t error_count = 0;
bool error_active = 0; 
Error_function current_error;

static uint8_t error_id[2] = { 0 };
static uint8_t error_data[16] = {[2 ... 15] = 'X'};

/* Functions ------------------------------------------------------------------- */

static void encode_err(Error_code error) {
	UART_encode(MAINBOARD_ERROR_ID, error_id);
	UART_encode((uint8_t) error, error_data);
}

//static void send_err_can2() {
//}

/**
 ******************************************************************************
 * @details			:	Error handling - turn off leds and while loop
 ******************************************************************************
 */
void Error_Handler(Error_function error_func, Error_code error_code) {
	__disable_irq();
	error_active = 1;
	
	if (error_count > 3) {
		while (1) {}
	}

	if (current_error == error_func) {
		error_count++;
		return;
	}
	
	if (current_error != 0) {
		// we have two different errors
		while (1) {}
	}

	current_error = error_func;
	encode_err(error_code);

	if (error_func != COMErrorFunc_BT) 
		BT_sendData(error_id, error_data);

	if (error_func != COMErrorFunc_ETH)
		Eth_sendData(error_id, error_data);

	switch (error_func) {
	case CAN1ErrorFunc_init:
		while (error_active) {
			error_active = 0;
			CAN1_Init();
		}

		break;

	case CAN1ErrorFunc_transfer:
		while (error_active) {
			error_active = 0;
			CAN1_Init();
			CAN1_transfer();
		}

		break;

	case CAN1ErrorFunc_fifo:
		break;

	case CAN2ErrorFunc_init:
		while (error_active) {
			error_active = 0;
			CAN2_Init();
		}		

		break;

	case CAN2ErrorFunc_transfer:
		while (error_active) {
			error_active = 0;
			CAN2_Init();
			CAN2_transfer();
		}

		break;

	case CAN2ErrorFunc_fifo:
		break;
	
	case COMErrorFunc_watchdogInit:
		break;

	case COMErrorFunc_BT:
		break;

	case COMErrorFunc_ETH:
		break;

	case TIM4ErrorFunc_init:
		while (error_active) {
			error_active = 0;
			TIM4_Init();
		} 
		break;

	case TIM7ErrorFunc_init:
		while (error_active) {
			error_active = 0;
			TIM7_Init();
		} 
		break;

	case TIM16ErrorFunc_init:
		while (error_active) {
			error_active = 0;
			MX_TIM16_Init();
		} 
		break;

	case SRCErrorFunc_init:
		break;

	case HALErrorFunc_uartInit:
		while (error_active) {
			error_active = 0;
			Eth_Init();
		} 
		break;
	}

	if (error_active == 1) {
		while (1) {}
	}
	error_count	= 0;
	error_func = 0; 

	__enable_irq();


	/// TODO: FInish Error handler!


//	Leds_init();
//	Leds_turnOff(LED_ALL);
//	Leds_turnOn(LED_4);
}
