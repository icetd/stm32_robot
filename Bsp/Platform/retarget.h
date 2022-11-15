#ifndef _RETARGET_H__
#define _RETARGET_H__

#include <stdio.h>

#pragma import(__use_no_semhosting)

int _write(int fd, char *ptr, int len);
void _sys_exit(int x);
void mPrintf(const char *fmt, ...);

#endif //#ifndef _RETARGET_H__