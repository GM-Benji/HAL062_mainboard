/* Includes ------------------------------------------------------------------*/

#include "camera.h"
#include "timers/timers.h"

/* Global variables -----------------------------------------------------------*/

int cam_bridge[3] = { 0, 0, 0 };

/* Functions ------------------------------------------------------------------*/

void Cam_Init(void) {

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, CAM_STER3_Pin | CAM_STER2_Pin | CAM_STER1_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pins : PCPin PCPin PCPin */
	GPIO_InitStruct.Pin = CAM_STER3_Pin | CAM_STER2_Pin | CAM_STER1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Cam_handle(uint8_t *data) {
	// Sprawdzenie czy dane spełniają warunki i włączenie odpowiedniej kamery

	for (uint8_t i=0; i<3; i++) {
		switch(data[i]){

		case 1 ... 3:
			cam_bridge[i] = data[i]-1;
			break;

		default:
			break;
		}
	}
}
