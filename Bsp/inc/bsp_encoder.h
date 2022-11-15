#ifndef BSP_ENCODER_H
#define BSP_ENCODER_H

#include <stdint-gcc.h>

#define ENCODER_MID_VALUE   30000

typedef struct encoder {
    uint16_t left_direction;
    uint16_t right_direction; 
    int16_t left_cnt;
    int16_t right_cnt;
    int16_t left_delta;
    int16_t right_delta;
    int16_t left_target;
    int16_t right_target;
} Encoder;

void encoder_init(void);
void encoder_set_counter(int8_t Motor_Num, int16_t count);
uint16_t encoder_get_counter(int8_t Motor_Num);
uint16_t encoder_get_dir(int8_t Motor_Num);

#endif