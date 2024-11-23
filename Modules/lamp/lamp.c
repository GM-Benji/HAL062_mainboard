#include <stm32h7xx_hal.h>
#include <string.h>
#include <lamp/lamp.h>
#include "error_handlers/error_handlers.h"

// look into merging this with the leds module

uint8_t Counter_red = 0;
uint8_t Counter_blue = 0;
uint8_t Counter_green = 0;
uint8_t maxCounterRed = 0;
uint8_t maxCounterBlue = 0;
uint8_t maxCounterGreen = 0;

void Lamp_setMaxValue(uint8_t *data) {
	maxCounterRed = data[3];
	maxCounterBlue = data[4];
	maxCounterGreen = data[5];
}

void Lamp_init() {
	// Inicjalizacja pinów diod
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = LAMP_1 | LAMP_2 | LAMP_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LAMP_PORT, &GPIO_InitStruct);
}

void Lamp_turnOn(uint32_t lampId) {
	HAL_GPIO_WritePin(LAMP_PORT, lampId, GPIO_PIN_SET);
}

void Lamp_turnOff(uint32_t lampId) {
	HAL_GPIO_WritePin(LAMP_PORT, lampId, GPIO_PIN_RESET);
}

void Lamp_toggle(uint32_t lampId) {
	HAL_GPIO_TogglePin(LAMP_PORT, lampId);
}

void Lamp_handle(uint8_t *data) {
	// Sprawdzenie czy dane spełniają warunki i włączenie odpowiedniej diody

	if (data[0] == 1) {
		Lamp_turnOn(LAMP_1);
	} else {
		Lamp_turnOff(LAMP_1);
	}

	if (data[1] == 1) {
		Lamp_turnOn(LAMP_2);
	} else {
		Lamp_turnOff(LAMP_2);
	}

	if (data[2] == 1) {
		Lamp_turnOn(LAMP_3);
	} else {
		Lamp_turnOff(LAMP_3);
	}
}

