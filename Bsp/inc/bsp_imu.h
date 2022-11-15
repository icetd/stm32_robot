#ifndef BSP_IMU_H
#define BSP_IMU_H

#include <stdint-gcc.h>

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} Gravity;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} Accel;

typedef struct {
    Gravity gravity;
    Accel accel;
    float yaw;
    float roll;
    float pitch;
    float base_yaw;
} Imu;

#ifdef __cplusplus
extern "C" {
#endif

void imu_init();
void imu_get_info(Imu *_imu);

#ifdef __cplusplus
}
#endif


#endif