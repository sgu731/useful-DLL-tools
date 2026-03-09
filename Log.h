#ifndef __LOG_H__
#define __LOG_H__

#include <Windows.h>
#include <stdio.h>

void Log(const wchar_t *fmt, ...);
void Hexdump(void *ptr, int buflen);

#endif
