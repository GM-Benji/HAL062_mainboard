#ifndef MODULE_LAMP_LAMP_H
#define MODULE_LAMP_LAMP_H

#define LAMP_1 111
#define LAMP_2 112
#define LAMP_3 113
#define LAMP_PORT GPIOC

void Lamp_setMaxValue(uint8_t *data);
void Lamp_init();
void MX_TIM16_Init(void);

void Lamp_turnOn(uint32_t lampId);
void Lamp_turnOff(uint32_t lampId);
void Lamp_toggle(uint32_t lampId);

void Lamp_handle(uint8_t *data);

#endif
