#include "overload.h"

#include "debug.h"

#include <stdio.h>

int sum_int(int a, int b)
{
    printf("Int sum\n");
    return (a + b);
}

double sum_double(double a, double b)
{
    printf("Double sum\n");
    return (a + b);
}

float sum_float(float a, float b)
{
    printf("Float print.\n");
    return (a + b);
}

int main()
{
    printf(FMT("######################## Test overload ####################\n", YELLOW));
    int    a = 10, b = 11;
    double ad = 14.649, bd = 21.34212312;
    float  af = 10.2445, bf = 15.325931;

    printf("Sum int: %d .\n", sum(a, b));
    printf("Sum Double: %f .\n", sum(ad, bd));
    printf("Sum float: %f .\n", sum(af, bf));

    return 0;
}