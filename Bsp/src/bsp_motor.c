#include "main.h"
#include "tim.h"
#include "bsp_motor.h"
#include "log.h"


#define LOG_TAG     "MOTOR"

void motor_init()
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    log_i("MOTOR\t initialize success.\r\n");
}

static void motorr_set_velocity(int16_t velocity)
{ 
    uint16_t temp;
    if (velocity > 0) {
       HAL_GPIO_WritePin(DRA_GPIO_Port, DRA_Pin, GPIO_PIN_SET);
       HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, GPIO_PIN_RESET);
       temp = velocity;
    }
    else if (velocity < 0) {
       HAL_GPIO_WritePin(DRA_GPIO_Port, DRA_Pin, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, GPIO_PIN_RESET);
       temp = - velocity;
    }
    else {
       HAL_GPIO_WritePin(DRA_GPIO_Port, DRA_Pin, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, GPIO_PIN_SET);
       temp = 0;
    }
    
    if (velocity > 950) 
        temp = 1000;
    
    if (abs(velocity) < 80)
        temp = 0;
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, temp);
}

static void motorl_set_velocity(int16_t velocity)
{
    uint16_t temp;
    if (velocity > 0) {
       HAL_GPIO_WritePin(DRB_GPIO_Port, DRB_Pin, GPIO_PIN_SET);
       HAL_GPIO_WritePin(ENB_GPIO_Port, ENB_Pin, GPIO_PIN_RESET);
       temp = velocity;
    }
    else if (velocity < 0) {
       HAL_GPIO_WritePin(DRB_GPIO_Port, DRB_Pin, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(ENB_GPIO_Port, ENB_Pin, GPIO_PIN_RESET);
       temp = - velocity;
    }
    else {
       HAL_GPIO_WritePin(DRB_GPIO_Port, DRB_Pin, GPIO_PIN_RESET);
       HAL_GPIO_WritePin(ENB_GPIO_Port, ENB_Pin, GPIO_PIN_SET);
       temp = 0;
    }
    
    if (velocity > 950)
        temp = 1000;

    if (abs(velocity) < 80)
        temp = 0;

    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, temp);
}

void motor_set_velocity(int8_t Motor_Num, int16_t velocity)
{
    switch (Motor_Num) {
    case 1:
        motorr_set_velocity(velocity);
        break;
    case 2:
        motorl_set_velocity(velocity);
        break;
    default:
        log_e("Motor_Num[%d] ERROR\r\n", Motor_Num);
    }
}