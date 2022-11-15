#include "bsp_kinematics.h"
#include "log.h"
#include <math.h>

#define LOG_TAG     "KINEMEATICS"

double liner_correction_factor  = 1.0;
double angular_correction_factor = 1.0;
/*每个脉冲对应的轮子实际走的直线距离系数 m/one_encoder */
double rate_encoder = (double)(WHELL_DIAMETER * PI)/(ENCODER_RESOLUTION * 4 * 30);

void kinematics_init(int16_t *robot_params)
{
    liner_correction_factor = (double)robot_params[0]/1000;
    angular_correction_factor = (double)robot_params[1]/1000;

    float r_x = D_X / 2;
    rate_encoder = (float)(WHELL_DIAMETER * PI)/(ENCODER_RESOLUTION * 4) * liner_correction_factor;
}

/** 
  * @brief  正向运动学解析，轮子编码值 --> 底盘三轴里程计坐标
  * 
  * @param  encoder 编码器信息  
  * @param  odom 底盘里程计 
  * 
  * @retval none           
  */
void kinematics_forward(Encoder encoder , Odom *odom) //正解 编码器 -> 底盘里程计
{
    static int16_t inc_encoder_l;
    static int16_t inc_encoder_r;
    static double dv_l;
    static double dv_r;
    static double dv;
    static double theta;

    inc_encoder_l = encoder.left_delta ;
    inc_encoder_r = encoder.right_delta ;
    
    dv_l = (double) inc_encoder_l * rate_encoder * ERR;
    dv_r = (double) inc_encoder_r * rate_encoder * ERR;
    

    dv = (float) (dv_l + dv_r) / 2;
    theta = (float) (dv_r - dv_l) / D_X;
    
    if ((dv_l * dv_r) < 0)
        dv = 0.0;
    //odom->yaw += theta; // use the imu
    
    if(odom->yaw >=  PI) {
        odom->yaw -= 2*PI;   
    } else if(odom->yaw <= -PI) {
        odom->yaw += 2*PI;
    }
  
    odom->x += dv * cos((double)odom->yaw);
    odom->y += dv * sin((double)odom->yaw);

    odom->dx =  dv * cos((double)odom->yaw);
    odom->dy =  dv * sin((double)odom->yaw);
    odom->dyaw = theta;

    //log_i("dv_l: %.6f, dv_r: %.6f\r\n", dv_l, dv_r);
}


/**
 * @brief  运动学逆解 ROS速度 -> 编码器差值   
 * @param  target_velocity  目标速度由ROS上位机发送
 * @param  encoder 编码器目标值
 * @retval none
 */
void kinematics_inverse(Target_velocity target_velocity, Encoder *encoder)
{
    float v_x;
    float theta;
    static float dv_l;
    static float dv_r;

    v_x = target_velocity.v_x;
    theta = target_velocity.theta;

    dv_l = v_x + theta * ((float)D_X / 2);
    dv_r = v_x - theta * ((float)D_X / 2);

    encoder->left_target =  (float)(dv_l / rate_encoder / PID_CORRECTION);
    encoder->right_target = (float)(dv_r / rate_encoder / PID_CORRECTION);
    //printf("dv_l: %f dv_r: %f TL:%d TR:%d\r\n", dv_l, dv_r, encoder->left_target, encoder->right_target);
}

