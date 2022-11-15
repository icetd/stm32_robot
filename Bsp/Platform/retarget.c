#include "usart.h"
#include <errno.h>
#include "log.h"
#include "usart.h"
#include <stdarg.h>
#include "cmsis_os.h"


int *__errno()
{
    log_e("error from libc.\r\n");
}

int _write(int fd, char *ptr, int len)
{ 
  return HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 0xFFFF); 
}

void _sys_exit(int x)
{
    x = x;
}

void mPrintf(const char *fmt, ...)
{
  uint8_t str[256];   
  uint16_t len;
  va_list args;
  va_start(args, fmt);
  len = vsprintf(str, fmt, args);
  HAL_UART_Transmit(&huart1, str, len, 0xFFFF);
  va_end(args);
}