#include "camera.h"
#include "timers/timers.h"

int cam_bridge[3] = { 0, 0, 0 };

void Camera_Init(void) {

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

	// why don't we throw an error?
	if ((3 >= data[0]) && (data[0] >= 1)) {
		cam_bridge[0] = data[0] - 1;
	}

	if ((3 >= data[1]) && (data[1] >= 1)) {
		cam_bridge[1] = data[1] - 1;
	}

	if ((3 >= data[2]) && (data[2] >= 1)) {
		cam_bridge[2] = data[2] - 1;
	}
}
