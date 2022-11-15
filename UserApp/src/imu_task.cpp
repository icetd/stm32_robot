#include "imu_task.h"
#include "bsp_imu.h"
#include "cmsis_os.h"
#include "bsp_kinematics.h"
#include "log.h"
#include <stdio.h>

#define LOG_TAG     "IMU"
#define CHECK_ERR   0.000038f

static int16_t outYaw;
Imu imu;

void imu_task(void const *argument)
{
    imu_get_info(&imu); 
    imu.base_yaw = imu.yaw;

    while(1)
    {
        imu_get_info(&imu);
        imu.yaw = -(imu.yaw - imu.base_yaw);
        imu.base_yaw -= CHECK_ERR;
        if (imu.yaw > 180.0) {
            imu.yaw -= 360.0;
        } else if (imu.yaw < (-180.0)) {
            imu.yaw += 360.0;
        }


        outYaw = (int16_t)imu.yaw;  //设置 odom YAW 的精度为 1°
        odom.yaw = (PI / 180.0f) * outYaw;
        osDelay(10);
    }
}