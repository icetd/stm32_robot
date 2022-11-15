#include "bsp_imu.h"
#include "main.h"
#include <stdio.h>
#include "i2c.h"
#include "cmsis_os.h"
#include "MPU6050.hpp"
#include "led_task.h"
#include "log.h"
#include "bsp_led.h"


#define LOG_TAG "IMU"

#define PI 3.1415926536f


static uint8_t g[3];
static float gyro_X, gyro_Y, gyro_Z, temp;
MPU6050 mpu;
static uint16_t packageSize;
static uint16_t fifoCount;
static uint8_t fifoBuffer[128];
static Quaternion q;
static VectorFloat gravity;
static float ypr[3];

static void imu_read_fifo()
{
    while(fifoCount < packageSize) {

        fifoCount = mpu.getFIFOCount();
    }
    if (fifoCount >= 1024) {
        mpu.resetFIFO();
    } else {
    if (fifoCount % packageSize != 0) {
            mpu.resetFIFO();
            fifoCount = mpu.getFIFOCount();
        } else {
            while (fifoCount >= packageSize) {
                mpu.getFIFOBytes(fifoBuffer, packageSize);
                fifoCount -= packageSize;
            }
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        }
    }
}

void imu_get_info(Imu *imu)
{
    imu_read_fifo();
    imu->accel.x = mpu.getAccelerationX(); 
    imu->accel.x = mpu.getAccelerationX(); 
    imu->accel.x = mpu.getAccelerationX(); 

    imu->gravity.x = gravity.x;
    imu->gravity.y = gravity.y;
    imu->gravity.z = gravity.z;

    imu->yaw = ypr[0] * 180.0f / PI;
    imu->pitch = ypr[1] * 180.0f / PI;
    imu->roll = ypr[2] * 180.0f / PI;
}

void imu_init()
{
    char ret;
    for (uint8_t i = 0; i < 255; i++) {
        if (HAL_I2C_IsDeviceReady(&hi2c1, i, 1, 10) == HAL_OK) {
            g[2] = i;
            break;
        }
    }
   osDelay(2000);
   mpu.initialize();
    if (mpu.dmpInitialize() == 0)
        led_state = 1;
    else {
        log_i("imu initialize success.\r\n");
        NVIC_SystemReset();
    }
   
    led_green_on();

    mpu.setXGyroOffset(318);
    mpu.setYGyroOffset(48);
    mpu.setZGyroOffset(25);

    mpu.setXAccelOffset(-2484);
    mpu.setYAccelOffset(-932);
    mpu.setZAccelOffset(1422);

    mpu.setDMPEnabled(true);
    packageSize = mpu.dmpGetFIFOPacketSize();
    fifoCount = mpu.getFIFOCount();
    log_i("packageSize:%d, fifoCount:%d\r\n", packageSize, fifoCount);

    for(int i = 0; i < 1500; i++) {
        imu_read_fifo();
        HAL_Delay(10);
    }
    led_state = 2;
}

