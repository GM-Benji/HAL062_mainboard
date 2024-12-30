/**
 ******************************************************************************
 * @file          	: communication.h
 * @author        	: Krystian Czechowicz, Adam Rybojad
 * @brief         	: Wireless communications functionality
 * @details			: This file contains both ethernet and bluetooth
 * 					  UART configuration. To undarstad it is required to see
 * 					  frame documentation. (You can see in communication.c)
 ******************************************************************************
 */

#ifndef MODULE_COMMUNICATION_COMMUNICATION_H
#define MODULE_COMMUNICATION_COMMUNICATION_H

/* Includes ------------------------------------------------------------------*/

#include <stm32h7xx_hal.h>
#include <stdbool.h>

/* Structures -----------------------------------------------------------------*/

struct commands {
	uint8_t messages[20][19];
	uint8_t numberOfCommands;
};

/* Functions ------------------------------------------------------------------*/

/**
 * *******************************************************************************
 * @brief	:	UART ethernet initialization
 * *******************************************************************************
 */
bool Eth_Init(void);

/**
 * *******************************************************************************
 * @brief	:	UART bluetooth initialization
 * *******************************************************************************
 */
bool BT_Init(void);

/**
 * *******************************************************************************
 * @brief		:	Sending data throgh bluetooth
 * @param ID	:	ID of data 
 * @param data	:	Information to send
 * *******************************************************************************
 */
void BT_sendData(uint8_t ID, uint8_t *data, uint8_t data_length);

/**
 * *******************************************************************************
 * @brief		:	Sending data throgh ethernet
 * @param ID	:	ID of data 
 * @param data	:	Information to send 
 * *******************************************************************************
 */
void Eth_sendData(uint8_t ID, uint8_t *data, uint8_t data_length);

/**
 * *******************************************************************************
 * @brief				:	Decoding data from UART
 * @param rawMessage	:	message from UART (bt or eth)
 * *******************************************************************************
 */
void UART_Decode(uint8_t *rawMessage);

/**
 * *******************************************************************************
 * @brief				:	Begin listening on ethernet UART
 * *******************************************************************************
 */
bool Eth_ReceiveData(void);

/**
 * *******************************************************************************
 * @brief				:	Begin listening on bluetooth UART
 * *******************************************************************************
 */
bool BT_ReceiveData(void);

/**
 * *******************************************************************************
 * @brief				:	Watchdog to control communication failure
 * *******************************************************************************
 */
void Watchdog_Init(void);

#endif // MODULE_COMMUNICATION_COMMUNICATION_H
