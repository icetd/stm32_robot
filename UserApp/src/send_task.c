#include "bsp_usart.h"
#include "usart.h"
#include "bsp_imu.h"
#include "bsp_kinematics.h"



extern Imu imu;
extern Odom odom;
extern Encoder encoder;
static int16_t sendData[15];


void send_to_com()
{
    sendData[0] = imu.gravity.x;
    sendData[1] = imu.gravity.y;
    sendData[2] = imu.gravity.z;

    sendData[3] = imu.accel.x;
    sendData[4] = imu.accel.y;
    sendData[5] = imu.accel.z;
    
    sendData[6] = imu.pitch * 100;
    sendData[7] = imu.roll * 100;
    sendData[8] = imu.yaw * 100;

    sendData[9] = odom.x * 100;
    sendData[10] = odom.y * 100;
    sendData[11] = odom.yaw * 100;
    sendData[12] = odom.dx * 100;
    sendData[13] = odom.dy * 100;
    sendData[14] = odom.dyaw * 100;

    usart_send(&huart3, sendData, 15, 06);
}

void send_task(void const *argument)
{
    while (1)
    {
        send_to_com();
        osDelay(20);
    }
}