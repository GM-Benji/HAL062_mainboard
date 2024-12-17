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

uint8_t ERROR_COUNT = 0;
bool ERROR_ACTIVE = 0;
uint8_t ERROR_CURRENT = 0;

/* Functions ------------------------------------------------------------------- */


/**
 * @see  documentation in the header file (error_handlers.h)
 */
void Error_Handler(Error_function error_func, Error_code error_code) {
	//__disable_irq();

	ERROR_ACTIVE = 1;

	if (ERROR_COUNT > 3) {
		while (1) {
		}
	}

	if (ERROR_CURRENT == error_func) {
		ERROR_COUNT++;
		return;
	}

	if (ERROR_CURRENT != 0) {
		// we have two different errors
		while (1) {
		}
	}

	ERROR_CURRENT = error_func;

	if (error_func != COMErrorFunc_Bt)
		BT_sendData((uint8_t) MAINBOARD_ERROR_ID, (uint8_t*) &error_code, 1);

	if (error_func != COMErrorFunc_Eth)
		Eth_sendData((uint8_t) MAINBOARD_ERROR_ID, (uint8_t*) &error_code, 1);

	switch (error_func) {
	case MAINEErrorFunc_test:
		ERROR_ACTIVE = 0;
		break;
	case CAN1ErrorFunc_init:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			CAN1_Init();
		}
		break;

	case CAN1ErrorFunc_transfer:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			CAN1_Init();
			CAN1_transfer();
		}
		break;

	case CAN1ErrorFunc_fifo:
		break;

	case CAN2ErrorFunc_init:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			CAN2_Init();
		}
		break;

	case CAN2ErrorFunc_transfer:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			CAN2_Init();
			CAN2_transfer();
		}
		break;

	case CAN2ErrorFunc_fifo:
		break;

	case COMErrorFunc_watchdogInit:
		break;

	case COMErrorFunc_BtInit:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			BT_Init();
		}
		break;

	case COMErrorFunc_EthInit:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			Eth_Init();
		}
		break;

	// TODO figure out watchdog
	case COMErrorFunc_Bt:
		ERROR_ACTIVE = 0;
		break;

	case COMErrorFunc_Eth:
		ERROR_ACTIVE = 0;
		break;

	case TIM4ErrorFunc_init:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			TIM4_Init();
		}
		break;

	case TIM7ErrorFunc_init:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			TIM7_Init();
		}
		break;

	case TIM16ErrorFunc_init:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			MX_TIM16_Init();
		}
		break;

	case SRCErrorFunc_init:
		break;

	default:
		break;
	}

	if (ERROR_ACTIVE == 1) {
		while (1) {
		}
	}
	ERROR_COUNT = 0;
	ERROR_CURRENT = 0;

	//__enable_irq();

	/// TODO: FInish Error handler!

//	Leds_init();
//	Leds_turnOff(LED_ALL);
//	Leds_turnOn(LED_4);
}
