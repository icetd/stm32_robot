#include "bsp_usart.h"
#include "bsp_kinematics.h"
#include "bsp_cmd.h"
#include "stm32f1xx_it.h"
#include "log.h"
#include "cmsis_os.h"
#include <stdlib.h>

#define LOG_TAG     "USART"
#define RECEIVELEN  256

extern DMA_HandleTypeDef hdma_usart3_rx;

static const uint8_t head[2] = {0x55, 0xaa};
static const uint8_t end[2] = {0x0d, 0x0a};

static SerialMessage sendMeassage, receiveMessage;

static uint8_t rx_robot_buf[RECEIVELEN];
static uint32_t rx_robot_count = 0;
static int16_t messageData[256];

static uint8_t rx_debug_buf[1];
extern Target_velocity target_velocity;
extern int16_t _kp_r, _ki_r, _kd_r;

static void (*OnRecvEnd)(uint8_t* data, uint16_t len);

uint8_t calCrc(uint8_t *vectorData, uint16_t len)
{
    uint8_t crc = 0;
    while (len--) {
        crc ^=  (*vectorData++);
        for (int i = 0; i < 8; i++) {
            if (crc & 0x01)
                crc = (crc >> 1) ^ 0x8C;
            else 
                crc >>= 1;
        }
    }
    return crc;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        if (rx_debug_buf[0] == 0x0d) {

        } else if (rx_debug_buf[0] == 0x0a) {
            cmd.flag = 1;
        } else {
            cmd.buffer[cmd.buffer_count] = rx_debug_buf[0];
            cmd.buffer_count++;
        }
        HAL_UART_Receive_IT(&huart1, rx_debug_buf, 1);
    }
}

static void OnUartCmd(uint8_t* _data, uint16_t _len)
{
    if(_data[0] == head[0] && _data[1] == head[1] && _data[_len-2] == end[0] && _data[_len-1] == end[1]) {
        
        if (_data[_len-3] == calCrc(_data, _len-3)) {
            for (int i = 0; i < _data[2]; i++) {
                for (int j = 0; j < 2; j++)
                    receiveMessage.data_transmit[j] = _data[i * 2 + 4 + j];
                messageData[i] = receiveMessage.data;
            }
            if (_data[3] == 11) {
                target_velocity.v_x = (float)messageData[0] / 1750;
                target_velocity.theta = -(float)messageData[1] / 750;
            }
        } 
    }
}

static void Uart_SetRxCpltCallBack(void (* xerc)(uint8_t* , uint16_t))
{
  OnRecvEnd = xerc;
}

void UART_IDLECallBack(UART_HandleTypeDef *huart)
{
    uint16_t temp;  
	
    if(huart->Instance == USART3)
	{
		if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart3);
            HAL_UART_DMAStop(&huart3);

            temp = __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
            rx_robot_count = RECEIVELEN - temp;

            OnRecvEnd(rx_robot_buf, rx_robot_count);
            HAL_UART_Receive_DMA(&huart3, rx_robot_buf, RECEIVELEN);
        }
    }
}

void usart_init()
{
    __HAL_UART_CLEAR_OREFLAG(&huart1); 
    HAL_UART_Receive_IT(&huart1, rx_debug_buf, 1);
    
    __HAL_UART_CLEAR_IDLEFLAG(&huart3); //初始化时清空IDEL中断标志,不然直接进中断，ram 不够freertos初始化。
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    Uart_SetRxCpltCallBack(OnUartCmd);
    HAL_UART_Receive_DMA(&huart3, rx_robot_buf, RECEIVELEN);
    
    log_i("Usart\t initialize success.\r\n");
}

uint8_t usart_send(UART_HandleTypeDef *huart, short *writeData, uint8_t messageLen, uint16_t messageType)
{
    uint8_t sendLength = (uint16_t)(2 * messageLen) + 7;
    uint8_t *sendBuf = malloc(sendLength * sizeof(uint8_t));
    memset(sendBuf, 0, sendLength);

    for (int i = 0; i < 2; i ++) {
        sendBuf[i] = head[i];
    }

    sendBuf[2] = sendLength;

    sendBuf[3] = messageType;

    for (int i = 0; i < messageLen; i++) {
        sendMeassage.data = writeData[i];
        for (int j = 0; j < 2; j++) {
            sendBuf[i * 2 + 4 + j] = sendMeassage.data_transmit[j];
        }
    }

    sendBuf[2 * messageLen + 4] = calCrc(sendBuf, 2 * messageLen + 4);

    sendBuf[2 * messageLen + 5] = end[0];
    sendBuf[2 * messageLen + 6] = end[1];
    
    HAL_UART_Transmit(huart, (uint8_t *)sendBuf, sendLength, 0xFFFF);
    sendMeassage.data = 0;

    free(sendBuf);
    return 1;
}

