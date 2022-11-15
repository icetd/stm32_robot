#include "bsp_led.h"
#include "log.h"

#define LOG_TAG     "LED"

void led_init()
{
    led_green_off();
    log_i("LED\t initialize success.\r\n");
}