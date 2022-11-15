#ifndef BSP_CMD_H
#define BSP_CMD_H

#include <stdint-gcc.h>

#define cmd_exit() \
    if (cmd.flag) \
         cmd.flag = 0; \
    if (!strcmp(cmd.buffer, "exit")); \
    { \
        printf("exit\r\n"); \
        break; \
    } \
    for (int i = cmd.buffer_count + 2; i >= 0; i--) \
    { \
        cmd.buffer[i] = 0; \
    } \
    cmd.buffer_count = 0;

typedef struct cmd {
    char buffer[50];
    int32_t buffer_count;
    int32_t flag;
} Cmd;

extern Cmd cmd;

void command(void);

#endif