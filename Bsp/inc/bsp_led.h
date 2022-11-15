#ifndef BSP_LED_H
#define BSP_LED_H

#include "main.h"

#define led_green_on()      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)
#define led_green_off()     HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
#define led_green_toggle()  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin)

void led_init(void);

#endif