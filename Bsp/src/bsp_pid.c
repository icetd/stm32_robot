#include "bsp_pid.h"
#include "log.h"

#define LOG_TAG     "PID"
#define PID_INTEGRAL_UP     1000
#define PID_SCALE           0.001f   //PID缩放系数


int16_t _kp_l;
int16_t _ki_l;
int16_t _kd_l;


int16_t _kp_r;
int16_t _ki_r;
int16_t _kd_r;

int16_t pid_control (int8_t Motor_Num, int16_t velocity_current, int16_t velocity_target)
{
    _kp_l = 220;
    _ki_l = 107;
    _kd_l = 10;

    _kp_r = 220;
    _ki_r = 107;
    _kd_r = 10;

    static int16_t motor_pwm[2];
    static int32_t err[2], err_last[2],  err_intergral[2];
    
    err[Motor_Num - 1] = velocity_target - velocity_current; // 测试是使用的编码器和电机方向相反
    err_intergral[Motor_Num - 1] += err[Motor_Num - 1];  

    if(velocity_target == 0) {
        err_intergral[Motor_Num - 1] = 0;
    }

    if(err_intergral[Motor_Num - 1] > PID_INTEGRAL_UP)
        err_intergral[Motor_Num - 1] = PID_INTEGRAL_UP;
    if(err_intergral[Motor_Num - 1] < -PID_INTEGRAL_UP)
        err_intergral[Motor_Num - 1] = -PID_INTEGRAL_UP;
 
    motor_pwm[0] = _kp_l  * err[Motor_Num - 1] * PID_SCALE + \
                              _ki_l * err_intergral[Motor_Num - 1] * PID_SCALE + \
                              _kd_l * (err[Motor_Num - 1] - err_last[Motor_Num - 1]) * PID_SCALE;

    motor_pwm[1] = _kp_r * err[Motor_Num - 1] * PID_SCALE + \
                              _ki_r * err_intergral[Motor_Num - 1] * PID_SCALE + \
                              _kd_r * (err[Motor_Num - 1] - err_last[Motor_Num - 1]) * PID_SCALE;

    err_last[Motor_Num  - 1] = err[Motor_Num - 1];

    if(motor_pwm[Motor_Num - 1] > 1000)
        motor_pwm[Motor_Num - 1] = 1000;
    if(motor_pwm[Motor_Num - 1] < -1000)
        motor_pwm[Motor_Num - 1] = -1000;
    
    if (Motor_Num  == 1)
        return -motor_pwm[Motor_Num - 1]; //电机和编码器转的相反
    if (Motor_Num == 2)
        return motor_pwm[Motor_Num - 1];
}