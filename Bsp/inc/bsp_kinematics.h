#ifndef BSP_KINEMATICS_H
#define BSP_KINEMATICS_H

#include <stdint-gcc.h>
#include "bsp_encoder.h"

#define COUNT_PLUSE 4
#define ENCODER_RESOLUTION  1000.0 //编码器分辨率，轮子转一圈，编码器的脉冲数
#define REDUCTION_RATIO 20  //电机减速比  
#define WHELL_DIAMETER  0.096 //轮子直径
#define D_X 0.368   //履带间距
#define PID_CORRECTION 100.0f  //PID 频率

#define PI 3.14159265358
#define ERR     (4.0 / 3) // 理论移动距离和电机克服摩擦后的修正

typedef struct target_velocity{
    float v_x;
    float theta;
} Target_velocity;

typedef struct odom{
    float x;
    float y;
    double yaw;
    float dx;
    float dy;
    float dyaw;
} Odom;

extern Odom odom;

void kinematics_init(int16_t *robot_params);
void kinematics_forward(Encoder encoder, Odom *odom); //正解 编码器 -> 底盘里程计
void kinematics_inverse(Target_velocity target_velocity, Encoder *encoder); //逆解 ROS速度 -> 轮子速度编码器目标

#endif

