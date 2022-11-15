#include "bsp_encoder.h"
#include "log.h"
#include "main.h"
#include "tim.h"

#define LOG_TAG     "ENCODER"


/**
 * @brief   encoder initialize
 * @param   motor num left:0  right:1
 */
void encoder_init(void)
{
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);  
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);  

    encoder_set_counter(1, ENCODER_MID_VALUE);
    encoder_set_counter(2, ENCODER_MID_VALUE);
    log_i("ENCODER\t initialize success.\r\n");
}

/**
 * @brief   set motor encoder count
 * @param   motor num left:0  right:1
 */
void encoder_set_counter(int8_t Motor_Num, int16_t count)
{
    switch(Motor_Num) {
    case 1: 
        __HAL_TIM_SET_COUNTER(&htim3, count);
        break;
    case 2: 
        __HAL_TIM_SET_COUNTER(&htim4, count);
        break;
    default:
        log_e("Motor_Num ERROR\r\n");
        break;
    }
}

/**
 * @brief   get motor encoder counts
 * @param   motor num left:0  right:1
 */
uint16_t encoder_get_counter(int8_t Motor_Num)
{
    uint16_t counter = 0;
    switch(Motor_Num) {
    case 1:
        counter = __HAL_TIM_GetCounter(&htim3);
        break;
    case 2:
        counter = __HAL_TIM_GetCounter(&htim4);
        break;

    default:
        log_e("Motor_Num ERROR\r\n");
        break;
    }
    return counter;
}

/**
 * @brief   get motor direction 
 * @param   motor num left:0  right:1
 */
uint16_t encoder_get_dir(int8_t Motor_Num)
{
    uint16_t direction = 1;
    switch (Motor_Num) {
    case 1:
        direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3);
        break;
    case 2:
        direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4);
        break;
    default:
        log_e("Motor_Num ERROR\r\n");
        break;
    }
    return direction;
}

