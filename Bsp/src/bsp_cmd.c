#include "bsp_cmd.h"
#include "main.h"
#include "log.h"
#include "cmsis_os.h"
#include "bsp_imu.h"
#include "bsp_kinematics.h"
#include "bsp_pid.h"
#include "cmd_task.h"
#include "retarget.h"
#include <string.h>
#include <stdlib.h>


Cmd cmd;
void split (char *src, const char * separator, char **dest, int *num) 
{
    char *pNext;
    int count = 0;
    if (src == NULL || strlen(src) == 0)
        return;

    if (separator == NULL || strlen(separator) == 0) 
        return;

   pNext = strtok(src, separator);
   while(pNext != NULL) {
        *dest ++;
        ++count; 
        pNext = strtok(NULL, separator);
   }
   *num = count;
}


static void reboot(void)
{
    NVIC_SystemReset();    
}
static void help(void)
{
    printf ("Commands: \r\n");
    printf ("hi                 ------------    Say hello to me\r\n");
    printf ("reboot             ------------    System reboot\r\n");
    printf ("help               ------------    Help list\r\n");
    printf ("debug_on           ------------    Show debug message\r\n");
    printf ("debug_off          ------------    Don't show debug message\r\n");
    printf ("show_imu           ------------    Show mpu6050 Pitch Roll Yaw\r\n");
    printf ("show_free          ------------    Show the free Heap\r\n");
    printf ("show_battery       ------------    Show battery\r\n");
    printf ("show_battery_once  ------------    Show battery once\r\n");
    printf ("show_send_to_pi    ------------    Show data sen to pi\r\n");
    printf ("set_pid            ------------    Show pid, format: [set_pid P I D]\r\n");
    printf ("set_velocity       ------------    Show veloctry, format: [set_velocity X(m/s) Yaw(rad/s)]\r\n");
}

static void debug_on()
{
    elog_set_filter_lvl(ELOG_LVL_DEBUG);
    printf("debug_on, elog_set_filter_lvl: ELOG_LVL_DEBUG\r\n");
}

static void debug_off()
{
    elog_set_filter_lvl(ELOG_LVL_INFO);
    printf("debug_off, elog_set_filter_lvl: ELOG_LVL_INFO\r\n");
}

static void show_free()
{
    printf("FreeHeapSize: %d\r\n", xPortGetFreeHeapSize());
}

static void show_imu()
{
   // extern struct imu_data imu;
    while(1) {
       //aprintf("Pitch: %.6f Roll: %.6f, Yaw: %.6f\r\n",
         //      imu.pitch, imu.roll, imu.yaw);
        
        cmd_exit();
        osDelay(20);
    }
}

static void show_battery()
{
    while(1) {
        printf("current voltage: \r\n");
        cmd_exit();
        osDelay(20);
    }
}

static void show_battery_once()
{
    printf("current voltage: \r\n");
}

static void show_send_to_pi()
{
    //extern int16_t sendData[];
    while(1) {
        for(int i = 0; i < 12; i++)
        //printf("%d\t", sendData[i]);
        cmd_exit();
        osDelay(10);
    }
    printf("\r\n");

}

static void set_pid()
{
    extern int16_t _kp_r;
    extern int16_t _ki_r;
    extern int16_t _kd_r;

    char *buf[4] = {0};
    int num = 0;
    split(cmd.buffer, " ",buf, &num);

    _kp_r = atoi(buf[1]);
    _ki_r = atoi(buf[2]);
    _kd_r = atoi(buf[3]);
 
    printf("\tSet motor pid to: \tP[%d]\tI[%d]\tD[%d]\t\r\n",
            _kp_r, _ki_r, _kd_r);
}

static void set_velocity()
{
    extern Target_velocity target_velocity;
    char *buf[2] = {0};
    int num = 0;
    split(cmd.buffer, " ", buf, &num);

    target_velocity.v_x = atof(buf[1]);
    target_velocity.theta = atof(buf[2]) * 3.1415926 / 180;

    printf("\tSet robot velocity to: \tX[%.2f m/s]\tYaw[%1.f rad/s]\t\r\n",
            atof(buf[1]), atof(buf[2]) * 3.1415926/180);
}

void command(void)
{
    
    if (cmd.flag)
        cmd.flag = 0;
    else 
        return;
    
    printf("cmd > \r\n");
    if (!strcmp(cmd.buffer, "hi"))  
        printf("hi there~\r\n");
    else if (!strcmp(cmd.buffer, "reboot"))
        reboot();
    else if (!strcmp(cmd.buffer, "help"))
        help();
    else if (!strcmp(cmd.buffer, "debug_on"))
        debug_on();
    else if (!strcmp(cmd.buffer, "debug_off"))
        debug_off();
    else if (!strcmp(cmd.buffer, "show_imu"))
        show_imu();
    else if (!strcmp(cmd.buffer, "show_battery"))
        show_battery();
    else if (!strcmp(cmd.buffer, "show_battery_once"))
        show_battery_once();
    else if (!strcmp(cmd.buffer, "show_send_to_pi"))
        show_send_to_pi();
    else if (!strcmp(cmd.buffer, "set_pid"))
        set_pid();
    else if (!strcmp(cmd.buffer, "set_velocity"))
        set_velocity();
    else if (!strcmp(cmd.buffer, "show_free"))
        show_free();
    else {
        printf("unknown commad '%s'.\r\n", cmd.buffer);
        help();
    }

    /*clear the cmd buf*/
    for (int i = cmd.buffer_count; i >=0 ; i--) {
        cmd.buffer[i] = 0;
    }
    cmd.buffer_count = 0;
}

