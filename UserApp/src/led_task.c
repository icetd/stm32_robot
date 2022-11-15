#include "cmsis_os.h"
#include "bsp_led.h"
#include "usart.h"

/** @brief led_state 
  * 0: LED_OFF
  * 1: LED_ON
  * 2: LED_TOGGLE 500ms 
  */
char led_state = 0;



void led_task(void const *argument)
{   
    while(1)
    {
        switch (led_state)
        {
        case 0:
            led_green_off();
            break;
        case 1:
            led_green_on();
            break;
        case 2:
            led_green_toggle();
            break;
        default:
            break;
        }
        osDelay(500);
    }
}