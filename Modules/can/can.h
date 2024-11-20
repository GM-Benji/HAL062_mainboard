/**
 ******************************************************************************
 * @file           : can.h
 * @author         : Jacek, Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Functionality of communication threw CAN
 ******************************************************************************
 */

#ifndef MODULES_CAN_CAN_H_
#define MODULES_CAN_CAN_H_


#include "leds/leds.h"

/* Structures ------------------------------------------------------------------*/

/**
 * *******************************************************************************
 * @brief		:	structure representing message FRAME passed to CAN
 * *******************************************************************************
 */
typedef struct {
	uint8_t ID;
	uint8_t lenght;
	uint8_t data[8];
} MessageTypeDef;


/* Functions ------------------------------------------------------------------*/

/**
 * *******************************************************************************
 * @brief		:	CAN1 initialization - outside (manipulator/lab)
 * *******************************************************************************
 */
void FDCAN1_Init(void);

/**
 * *******************************************************************************
 * @brief		:	CAN2 initialization - rail (motorboards/sensorboards)
 * *******************************************************************************
 */
void FDCAN2_Init(void);

/**
 * *******************************************************************************
 * @brief		:	Send test message via CAN1
 * *******************************************************************************
 */
void Can_testMessage(void);

/**
 * *******************************************************************************
 * @brief		:	Saves message to data struct, and calls transferTo function,
 * 					which pass message to appropriate CAN
 * *******************************************************************************
 */
void COM_RunUartAction(MessageTypeDef *message);

/**
 * *******************************************************************************
 * @brief		:	Checks ID of message and pass to CAN1 or CAN2
 * *******************************************************************************
 */
void transferTo(void);


#endif /* MODULES_CAN_CAN_H_ */
