#ifndef BSP_USART_H
#define BSP_USART_H

#include "usart.h"
#include <stdint-gcc.h>

typedef union serialMessage
{
    int16_t data;
    uint8_t data_transmit[2];
}SerialMessage;


void usart_init();
void UART_IDLECallBack(UART_HandleTypeDef *huart);
uint8_t usart_send(UART_HandleTypeDef *huart, short *writeData, uint8_t messageLen, uint16_t messageType);
uint8_t calCrc(uint8_t *vectorData, uint16_t len);

#endif