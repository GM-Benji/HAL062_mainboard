/**
 ******************************************************************************
 * @file           : error_handlers.h
 * @author         : Jacek Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Handling all errors that is detected TODO
 ******************************************************************************
 */

#ifndef ERROR_HANDLERS_ERROR_HANDLERS_H_
#define ERROR_HANDLERS_ERROR_HANDLERS_H_

typedef enum {
	CAN1Error_init,
	CAN1Error_configFilter,
	CAN1Error_start,

	CAN1Error_transferAddMessege,
	CAN1Error_transferEnableTx,
	CAN1Error_fifoGetMessege,
	CAN1Error_fifoActivateNotification,

	CAN2Error_init,
	CAN2Error_configFilter,
	CAN2Error_start,

	CAN2Error_transferAddMessege,
	CAN2Error_transferEnableTx,
	CAN2Error_fifoGetMessege,
	CAN2Error_fifoActivateNotification,

    TIM4Error_baseInit,
    TIM4Error_configClock,
    TIM4Error_OCInit,
    TIM4Error_configSync,
    TIM4Error_configChannel,
    TIM7Error_baseInit,
    TIM7Error_configSync,

	SRCError_mainOscConfig,
	SRCError_mainClockConfig,
    HALError_CAN1,
    HALError_CAN2,
    HALError_UART1PeriphClock,
    HALError_UART1RX,
    HALError_UART1TX
} Error_code;
/* Functions ----------------------------------------------------------------- */

void Error_Handler(void);

#endif /* ERROR_HANDLERS_ERROR_HANDLERS_H_ */
