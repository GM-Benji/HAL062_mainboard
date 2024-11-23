#ifndef MODULE_CAMERA_CAMERA_H
#define MODULE_CAMERA_CAMERA_H

#include <stm32h7xx_hal.h>

void Camera_Init(void);

void Cam_handle(uint8_t *data);

#endif // MODULE_CAMERA_CAMERA_H
