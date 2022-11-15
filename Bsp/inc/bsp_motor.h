#ifndef BSP_MOTOR_H
#define BSP_MOTOR_H

#include <stdint-gcc.h>

void motor_init(void);
void motor_set_velocity(int8_t Motor_Num, int16_t velocity);

#endif