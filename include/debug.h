#ifndef DEBUG__H
#define DEBUG__H

#include <stdio.h>

#define RED             "\033[31;1m"
#define YELLOW          "\033[33;1m"
#define NONE            "\033[0m"

#define FMT(str, color) color str NONE

#define error(x)                                                                           \
    do                                                                                     \
    {                                                                                      \
        printf("\033[31;1m %s %s %d: Error " x "\033[0m\n", __FILE__, __func__, __LINE__); \
        return -1;                                                                         \
    }                                                                                      \
    while (0)

#endif