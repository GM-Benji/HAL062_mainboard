/**
 ******************************************************************************
 * @file           : leds.c
 * @author         : J. Prokopczuk
 * @brief          : Leds driver
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_rcc.h>
#include <stm32h7xx_hal_gpio.h>
#include <stm32h7xx_hal.h>
#include "leds/leds.h"


/*
 * ***************************************************
 * @ details	:	Initialization of GPIO using
 * 					only predefined macros
 * ***************************************************
 */
void Leds_init(void) {
	GPIO_InitTypeDef gpio;
	__HAL_RCC_GPIOG_CLK_ENABLE();
	gpio.Pin = LED_1 | LED_2 | LED_3 | LED_4 | LED_5;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_PULLDOWN;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &gpio);
}
/* Functions ------------------------------------------------------------------*/


/*
 * ***************************************************
 * @ details	:	High level led turning on, using
 * 					only predefined macros
 * ***************************************************
 */
void Leds_turnOnLed(uint32_t ledId) {
	HAL_GPIO_WritePin(LEDS_PORT, ledId, 0x01u);
}

/*
 * ***************************************************
 * @ details	:	High level led turning off, using
 * 					only predefined macros
 * ***************************************************
 */
void Leds_turnOffLed(uint32_t ledId) {
	HAL_GPIO_WritePin(LEDS_PORT, ledId, 0u);
}

/*
 * *************************************************************
 * @ details	:	Turn on all LEDs. 
 * *************************************************************
 */
void Leds_turnOnAllLeds(void) {
	uint32_t reg = LED_1 | LED_2 | LED_3 | LED_4 | LED_5;
	HAL_GPIO_WritePin(LEDS_PORT, reg, 0x01u);
}

/*
 * *************************************************************
 * @ details	:	Turn off all LEDs. 
 * *************************************************************
 */
void Leds_turnOffAllLeds(void) {
	uint32_t reg = LED_1 | LED_2 | LED_3 | LED_4 | LED_5;
	HAL_GPIO_WritePin(LEDS_PORT, reg, 0u);
}

/*
 * *****************************************************
 * @ details	:	Toggling LED using predefined macro
 * *****************************************************
 */
void Leds_toggleLed(uint32_t ledId) {
	HAL_GPIO_TogglePin(LEDS_PORT, ledId);
}

/*
 * *****************************************************
 * @ details	:	Function used to check LEDs performing
 * 				called at the beginning of program.
 * *****************************************************
 */
void Leds_welcomeFLash(void) {
	Leds_turnOnAllLeds();
	HAL_Delay(1000);

	Leds_turnOffAllLeds();
	HAL_Delay(1000);
}
