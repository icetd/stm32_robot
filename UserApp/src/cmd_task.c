#include "log.h"
#include "cmsis_os.h"
#include "bsp_cmd.h"

#define LOG_TAG     "CMD"

void cmd_task(void const *argument)
{
    while(1)
    {
        command();
        osDelay(20);
    }
}