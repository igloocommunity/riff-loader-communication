/*******************************************************************************
 * Copyright (C) ST-Ericsson SA 2011
 * License terms: 3-clause BSD license
 ******************************************************************************/

#include <time.h>
#include <errno.h>
#include "Types.h"
#include "OS.h"

DWORD OS::ErrorCode = 0;

OS::OS()
{
}

OS::~OS()
{
}

void OS::Sleep(DWORD dwMilliseconds)
{
    struct timespec ts;

    ts.tv_sec = dwMilliseconds / 1000;
    ts.tv_nsec = (dwMilliseconds % 1000) * 1000000L;

    /* coverity[returned_null] */
    while (-1 == nanosleep(&ts, &ts) && errno == EINTR);
}

time_t OS::GetSystemTimeInMs()
{
    timespec systemTime;
    clock_gettime(CLOCK_REALTIME, &systemTime);
    return (systemTime.tv_sec * 1000) + (systemTime.tv_nsec / 1000000);
}

timespec OS::GetAbsoluteTime(DWORD dwTimeout)
{
    timespec absolute_time;
    timespec current_time;

    long timeout_nsec;

    clock_gettime(CLOCK_REALTIME, &current_time);

    absolute_time.tv_sec = current_time.tv_sec + (dwTimeout / 1000);
    timeout_nsec = (dwTimeout % 1000) * 1000000L;

    if ((1000000000 - current_time.tv_nsec) < timeout_nsec) {
        // overflow will occur!
        absolute_time.tv_sec++;
    }

    absolute_time.tv_nsec = (current_time.tv_nsec + timeout_nsec) % 1000000000;

    return absolute_time;
}


char *strcpy_s(char *dst, size_t _Size, const char *src)
{
    return strncpy(dst, src, _Size);
}

int sprintf_s(char *dst, size_t _Size, const char *format, ...)
{
    va_list l;
    int ReturnValue;

    va_start(l, format);
    ReturnValue = vsnprintf(dst, _Size, format, l);
    va_end(l);

    return ReturnValue;
}

char *strncpy_s(char *dst, const char *src, size_t _Size)
{
    return strncpy(dst, src, _Size);
}

int _stricmp(const char *s1, const char *s2)
{
    return strcasecmp(s1, s2);
}

