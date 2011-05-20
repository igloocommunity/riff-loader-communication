/*******************************************************************************
 * Copyright (C) ST-Ericsson SA 2011
 * License terms: 3-clause BSD license
 ******************************************************************************/

#ifndef _OS_H
#define _OS_H
#include <sys/time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "Types.h"

#include "CWaitableObject.h"

class OS
{
public:
    OS();
    virtual ~OS();

    static void Sleep(DWORD dwMilliseconds);
    static time_t GetSystemTimeInMs();

    static DWORD GetErrorCode() {
        return OS::ErrorCode;
    }
    static void SetErrorCode(DWORD dwErrorCode) {
        OS::ErrorCode = dwErrorCode;
    }

    static timespec GetAbsoluteTime(DWORD dwTimeout);
private:

    static DWORD ErrorCode;
};

#define MAX(a, b) ((a) > (b) ? (a) : (b))

/*
 * str manipulation functions used in windows build
 */

char *strcpy_s(char *dst, size_t _Size, const char *src);
char *strncpy_s(char *dst, const char *src, size_t _Size);
int _stricmp(const char *s1, const char *s2);
int sprintf_s(char *dst, size_t _Size, const char *format, ...);

#define _snprintf snprintf

template <size_t _Size>
char *strcpy_s(char(&dst)[_Size], const char src[])
{
    return strncpy(dst, src, _Size);
}

template<size_t _Size>
int sprintf_s(char(&dst)[_Size], const char *format, ...)
{
    int ReturnValue;
    va_list l;
    va_start(l, format);
    ReturnValue = vsnprintf(dst, _Size, format, l);
    va_end(l);
    return ReturnValue;
}

template <size_t _Size>
char *strcat_s(char(&dst)[_Size], const char src[])
{
    return strncat(dst, src, _Size);
}

template <size_t _Size>
int _ultoa_s(unsigned long value, char(&str)[_Size], int radix)
{
    switch (radix) {
    case 10:
        return sprintf_s(str, "%ul", value);
    case 16:
        return sprintf_s(str, "%ulX", value);
    default:
        return -1;
    }
}

template<size_t _Size>
int _snprintf_s(char(&dst)[_Size], size_t _MaxCount, const char *format, ...)
{
    int ReturnValue;
    va_list l;
    va_start(l, format);
    ReturnValue = vsnprintf(dst, MAX(_MaxCount, _Size), format, l);
    va_end(l);
    return ReturnValue;
}

template<size_t _Size>
int vsprintf_s(char(&dst)[_Size], const char *format, va_list l)
{
    return vsnprintf(dst, _Size, format, l);
}

#endif /* _OS_H */

