#ifndef BSP_PID_H
#define BSP_PID_H

#include <stdint-gcc.h>
#include "bsp_motor.h"


int16_t pid_control (int8_t Motor_Num, int16_t velocity_current, int16_t velocity_target);

#endif