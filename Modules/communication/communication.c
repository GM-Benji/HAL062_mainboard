/**
 ******************************************************************************
 * @file          	: communication.c
 * @author        	: Krystian Czechowicz, Adam Rybojad
 * @brief         	: Wireless communications functionality
 * @details			: This file contains both ethernet and bluetooth
 * 					  UART configuration. To undarstad it is required to see
 * 					  frame documentation. (You can see in communication.c)
 ******************************************************************************
 */

#include <stm32h7xx_hal.h>
#include <string.h>
#include "can/can.h"
#include "communication/communication.h"
#include "camera/camera.h"
#include "lamp/lamp.h"
#include "error_handlers/error_handlers.h"

static uint32_t err_counter = 0;
extern MessageTypeDef UART_MessageRecieved; // struct from can.h representing message

UART_HandleTypeDef btHuart;
UART_HandleTypeDef ethHuart;

IWDG_HandleTypeDef hiwdg1;

uint8_t UART_ReceivedRaw[19]; // check frame documentation
uint8_t searching = 0u;
uint8_t magnetosearching = 0u;
uint8_t tutaj = 0u;

DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

struct commands uartCommands;

static void encode_list_KNR(uint8_t *data, uint8_t *encoded,
		uint8_t data_length);
static uint8_t decode_list_KNR(uint8_t *encoded, uint8_t *data,
		uint8_t data_length);

/**
 * *******************************************************************************
 * @brief	:	Overwritten callback after receiving message
 * *******************************************************************************
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	
	// take advantage of both UART coms transfering the same information 
	HAL_IWDG_Refresh(&hiwdg1);
	UART_Decode(UART_ReceivedRaw);

	switch (UART_MessageRecieved.ID) {
	case 10:
		Lamp_handle(UART_MessageRecieved.data);
		Lamp_setMaxValue(UART_MessageRecieved.data);
		break;

	case 45:
		Cam_handle(UART_MessageRecieved.data);
		break;

	default:
		COM_RunUartAction(&UART_MessageRecieved);
		break;
	}

	if (huart->Instance == USART1) {
		// Handle Eth overwrite 
		UART_MessageRecieved.ID = 0;
		memset(&UART_MessageRecieved.data, 0x0u, 8);
		HAL_UART_Receive_DMA(&ethHuart, UART_ReceivedRaw, 19);
		return;

	} else if (huart->Instance == USART3) {
		// Handle BT overwrite
		UART_MessageRecieved.ID = 0;
		memset(&UART_MessageRecieved.data, 0x0u, 8);
		HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 19);
		return;

	}
}
// Isnt this important?
//
//	if (huart->Instance == USART1) {
//		UART_Decode(UART_ReceivedRaw);
//		if (searching == 0) {
//			COM_RunUartAction(&UART_MessageRecieved);
//			UART_MessageRecieved.ID = 0;
//			memset(&UART_MessageRecieved.data, 0x0u, 8);
//			HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 19);
//			return;
//		}
//		if (searching == 1) {
//			HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 1);
//			return;
//		}
//		if (searching == 2) {
//			HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 18);
//			return;
//		}
//	}
//	else if(huart->Instance == USART3)
//	{
//		UART_Decode(UART_ReceivedRaw);
//		if (searching == 0) {
//			COM_RunUartAction(&UART_MessageRecieved);
//			UART_MessageRecieved.ID = 0;
//			memset(&UART_MessageRecieved.data, 0x0u, 8);
//			HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 19);
//			return;
//		}
//		if (searching == 1) {
//			HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 1);
//			return;
//		}
//		if (searching == 2) {
//			HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 18);
//			return;
//		}
//	}
//}

/**
 * *******************************************************************************
 * @details		: Initialization of USART3 with FIFO Queue
 * *******************************************************************************
 */
bool BT_Init(void) {
	btHuart.Instance = USART3;
	btHuart.Init.BaudRate = 115200;
	btHuart.Init.WordLength = UART_WORDLENGTH_8B;
	btHuart.Init.StopBits = UART_STOPBITS_1;
	btHuart.Init.Parity = UART_PARITY_NONE;
	btHuart.Init.Mode = UART_MODE_TX_RX;
	btHuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	btHuart.Init.OverSampling = UART_OVERSAMPLING_16;
	btHuart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	btHuart.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	btHuart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&btHuart) != HAL_OK) {
//		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&btHuart, UART_TXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
//		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&btHuart, UART_RXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
//		Error_Handler();
	}

	// Most likely the error is here, this doesnt line up with NEW_ERA
	if (HAL_UARTEx_DisableFifoMode(&btHuart) != HAL_OK) { 	
//		Error_Handler();
	}

	return 0;
}

/**
 * *******************************************************************************
 * @details		: Initialization of USART1 with FIFO Queue
 * *******************************************************************************
 */
bool Eth_Init() {

	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
	/* DMA1_Stream1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

	/* USART1 interrupt Init */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	ethHuart.Instance = USART1;
	ethHuart.Init.BaudRate = 115200;
	ethHuart.Init.WordLength = UART_WORDLENGTH_8B;
	ethHuart.Init.Parity = UART_PARITY_NONE;
	ethHuart.Init.StopBits = UART_STOPBITS_1;
	ethHuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	ethHuart.Init.OverSampling = UART_OVERSAMPLING_16;
	ethHuart.Init.Mode = UART_MODE_TX_RX;

	HAL_UART_Init(&ethHuart);
	HAL_UARTEx_SetRxFifoThreshold(&ethHuart, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_SetTxFifoThreshold(&ethHuart, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_EnableFifoMode(&ethHuart);

	return 0;
}

/**
 * *******************************************************************************
 * @details		:	Initialization of watchdog that will reset board
 * 					after communcication failure
 * *******************************************************************************
 */
void Watchdog_Init(void) {
	hiwdg1.Instance = IWDG1;
	hiwdg1.Init.Prescaler = IWDG_PRESCALER_32;
	hiwdg1.Init.Window = 4095;
	hiwdg1.Init.Reload = 4095;
	if (HAL_IWDG_Init(&hiwdg1) != HAL_OK) {
		Error_Handler(COMErrorFunc_watchdogInit, COMError_watchdogInit);
		return;
	}
}

/**
 * *******************************************************************************
 * @details		:	Creating table of chars contains:
 * 					# - all message contains "#" at begging
 * 					ID - all data needs to have ID to identify what
 * 						 concrete data means
 * 					info - depending on ID, info contains concrete data
 * @see			:	UART frame documentation
 * *******************************************************************************
 */
void Eth_sendData(uint8_t ID, uint8_t data[8], uint8_t data_length) {
	uint8_t TxBuffer[19] = { [0] = '#', [1 ... 18] = 'X' };

	encode_list_KNR(&ID, TxBuffer + 1, 1);
	encode_list_KNR(data, TxBuffer + 3, data_length);

	if (HAL_UART_Transmit(&btHuart, TxBuffer, 19, 50) != HAL_OK) {
		// mb add error handler here	
	}
}

/**
 * *******************************************************************************
 * @details		:	Creating table of chars contains:
 * 					# - all message contains "#" at begging
 * 					ID - all data needs to have ID to identify what
 * 						 concrete data means
 * 					info - depending on ID, info contains concrete data
 * @see			:	UART frame documentation
 * *******************************************************************************
 */
void BT_sendData(uint8_t ID, uint8_t data[8], uint8_t data_length) {
	uint8_t TxBuffer[19] = { [0] = '#', [1 ... 18] = 'X' };

	encode_list_KNR(&ID, TxBuffer + 1, 1);
	encode_list_KNR(data, TxBuffer + 3, data_length);

	if (HAL_UART_Transmit(&btHuart, TxBuffer, 19, 50) != HAL_OK) {
		// mb add error handler here	
	}
}

/**
 * *******************************************************************************
 * @details		:	starting listening on bluetooth UART pins
 * *******************************************************************************
 */
bool BT_ReceiveData() {
	if (HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 19) == HAL_OK)
		return 0;
	return 1;
}

/**
 * *******************************************************************************
 * @details		:	starting listening on bluetooth UART pins
 * *******************************************************************************
 */
bool Eth_ReceiveData() {
	if (HAL_UART_Receive_DMA(&ethHuart, UART_ReceivedRaw, 19) == HAL_OK)
		return 0;
	return 1;
}

/**
 * *******************************************************************************
 * @details		:	all data are sent as HEX number coded in ASCII
 * 					(f.e number 140 is 8C so we send 0x38 (8 in ASCII)
 * 					and 0x43 (C in ASCII)
 * @see			:	UART frame documentation
 * *******************************************************************************
 */
void UART_Decode(uint8_t *rawMessage) {

	// WTH does this do
	if (rawMessage[0] != '#' && searching != 2) {
		searching = 1;
		return;
	}
	if (rawMessage[0] != '#' && searching == 1) {
		searching = 2;
		tutaj = 1;
		return;
	}
	if (searching == 2) {
		//nie ma '#' w tym wiec trzeba do gory jedno przeniesc
		for (int p = 17; p >= 0; p--) {
			rawMessage[p + 1] = rawMessage[p];
		}
		searching = 0;
	}

	/*Zamiana hex w ACSII na liczbe*/

	decode_list_KNR(rawMessage + 1, &UART_MessageRecieved.ID, 1);

	UART_MessageRecieved.lenght = decode_list_KNR(rawMessage + 3,
			UART_MessageRecieved.data, 8);
}

/**
 * *******************************************************************************
 * @brief		:	Counting UART_Error.  
 *********************************************************************************
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {

	if (huart->Instance == btHuart.Instance) {
		Error_Handler(COMErrorFunc_Bt, COMError_Bt);
		return;
	}

	if (huart->Instance == ethHuart.Instance) {
		Error_Handler(COMErrorFunc_Eth, COMError_Eth);
		return;
	}
	err_counter++;
}

static void encode_list_KNR(uint8_t *data, uint8_t *encoded,
		uint8_t data_length) {
	const uint8_t HEX_DIGITS[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
			'8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	for (uint8_t i = 0; i < data_length; i++) {
		encoded[i * 2] = HEX_DIGITS[(data[i] >> 4)];
		encoded[i * 2 + 1] = HEX_DIGITS[(data[i] & 0x0F)];
	}
}

static uint8_t decode_list_KNR(uint8_t *encoded, uint8_t *data,
		uint8_t data_length) {
	for (uint8_t i = 0; i < data_length; i++) {

		// decode first 4 bytes
		switch (encoded[i * 2]) {
		case '0' ... '9':
			data[i] = (encoded[i * 2] - 0x30) << 4;
			continue;

		case 'A' ... 'F':
			data[i] = (encoded[i * 2] - 0x37) << 4;
			continue;

		case 'Z': // end of message
			return i;

		default:
			return 0;
		}

		// decode last 4 bytes
		switch (encoded[i * 2 + 1]) {
		case '0' ... '9':
			data[i] = (encoded[i * 2 + 1] - 0x30) << 4;
			continue;

		case 'A' ... 'F':
			data[i] = (encoded[i * 2 + 1] - 0x37) << 4;
			continue;

		default:
			return 0;
		}
	}
	return data_length;
}
