#include "cmsis_os.h"
#include "bsp_kinematics.h"
#include "bsp_motor.h"
#include "bsp_pid.h"
#include "log.h"

#define LOG_TAG     "move"

static int16_t motor_pwm_l, motor_pwm_r;
Encoder encoder = {0};
Odom odom = {0};
Target_velocity target_velocity = {0};
char move_flag = 0;

extern int isUsart3TiSucess;

void move_task(void const *argument)
{   
    while(1)
    {
        encoder.left_delta  =  (encoder_get_counter(1) - ENCODER_MID_VALUE);
        encoder.left_direction = encoder_get_dir(1);
        
        encoder.right_delta  = (encoder_get_counter(2) - ENCODER_MID_VALUE);
        encoder.left_direction = encoder_get_dir(2);
        
        encoder_set_counter(1, ENCODER_MID_VALUE);
        encoder_set_counter(2, ENCODER_MID_VALUE);

        encoder.left_cnt = encoder.left_cnt + encoder.left_delta;
        encoder.right_cnt = encoder.right_cnt + encoder.right_delta;

       // target_velocity.theta = 0.2 * 2;
       // target_velocity.v_x = 0.1 * 2;

        kinematics_forward(encoder, &odom);
        kinematics_inverse(target_velocity, &encoder);

        motor_pwm_l = pid_control(1, encoder.left_delta, encoder.left_target);  
        motor_pwm_r = pid_control(2, encoder.right_delta, encoder.right_target);               

        //printf("TL:%d  TR:%d | L:%d R:%d | ML:%d MR:%d\r\n", encoder.left_target , encoder.right_target, encoder.left_delta, encoder.right_delta, motor_pwm_l, motor_pwm_r);
        //printf("odom_x:%f odom.y:%f odom_yaw:%f dx:%f, dy:%f, dyaw:%f\r\n", odom.x, odom.y, odom.yaw, odom.dx, odom.dy, odom.dyaw);
        //printf("v_x:%.1f v_yaw:%.1f\r\n", target_velocity.v_x, target_velocity.theta);
        motor_set_velocity(1, motor_pwm_l);
        motor_set_velocity(2, motor_pwm_r);

        osDelay(10);

    }  
}
