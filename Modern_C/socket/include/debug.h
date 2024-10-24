#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "macro.h"

#include <assert.h>
#include <stdio.h>

#define Log(format, ...) \
    _Log(ANSI_FMT("[%s:%d %s] " format, ANSI_FG_BLUE) "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define Assert(cond, format, ...)                                                     \
    do                                                                                \
    {                                                                                 \
        if (!(cond))                                                                  \
        {                                                                             \
            _Log(ANSI_FMT("[%s:%d %s] ", ANSI_FG_RED), __FILE__, __LINE__, __func__); \
            printf(ANSI_FMT(format, ANSI_FG_RED) "\n", ##__VA_ARGS__);                \
            assert(cond);                                                             \
        }                                                                             \
    }                                                                                 \
    while (0)

#define panic(format, ...) Assert(0, format, ##__VA_ARGS__)

#define Error(format, ...) panic("\"" format "\" Error.\n");

#define ARRLEN(arr)        sizeof((arr)) / sizeof(arr[0])

#define CHECK_RET(ret, msg) \
    do                      \
    {                       \
        if ((ret) == -1)    \
            Error(msg);     \
    }                       \
    while (0)

#endif
