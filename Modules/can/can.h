/**
 ******************************************************************************
 * @file           : can.h
 * @author         : Jacek, Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Functionality of communication threw CAN
 ******************************************************************************
 */

#ifndef MODULES_CAN_CAN_H_
#define MODULES_CAN_CAN_H_

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

union Angle {
	uint32_t ui;
	float f;
};

union Speed {
	uint32_t ui;
	float f;
};

/* Functions ------------------------------------------------------------------*/

/**
 * *******************************************************************************
 * @brief		:	CAN1 initialization - outside (manipulator/lab)
 * *******************************************************************************
 */
void CAN1_Init(void);

/**
 * *******************************************************************************
 * @brief		:	CAN2 initialization - rail (motorboards/sensorboards)
 * *******************************************************************************
 */
void CAN2_Init(void);

/**
 * *******************************************************************************
 * @brief		:	Send test message via CAN1
 * *******************************************************************************
 */
void CAN_testMessage(void);

/**
 * *******************************************************************************
 * @brief		:	Saves message to data struct, and calls transferTo function,
 * 					which pass message to appropriate CAN
 * *******************************************************************************
 */
void COM_RunUartAction(MessageTypeDef *message);

void CAN1_transfer(void);
void CAN2_transfer(void);

#endif /* MODULES_CAN_CAN_H_ */
