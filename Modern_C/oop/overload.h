#ifndef OVERLOAD__H
#define OVERLOAD__H

int    sum_int(int a, int b);

double sum_double(double a, double b);

float  sum_float(float a, float b);

#define sum(x, y) _Generic((x), \
    int: sum_int, \
    double: sum_double,\
    float : sum_float,\
    default: sum_int )((x),(y))

#endif