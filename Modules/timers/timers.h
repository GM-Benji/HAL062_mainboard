/**
 ******************************************************************************
 * @file           : timers.h
 * @author         : Krystian Czechowicz
 * @brief          : Timer to handle communication
 ******************************************************************************
 */

#ifndef MODULES_TIMERS_TIMERS_H_
#define MODULES_TIMERS_TIMERS_H_

#define CAM_STER3_Pin GPIO_PIN_0
#define CAM_STER3_GPIO_Port GPIOC
#define CAM_STER2_Pin GPIO_PIN_1
#define CAM_STER2_GPIO_Port GPIOC
#define CAM_STER1_Pin GPIO_PIN_2
#define CAM_STER1_GPIO_Port GPIOC

void TIM4_Init(void);
void TIM7_Init(void);
void MX_TIM16_Init(void);

#endif /* MODULES_TIMERS_TIMERS_H_ */
