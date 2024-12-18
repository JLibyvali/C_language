#include "point.h"

#include <math.h>

double length(TwoD *_point)
{
    if (!_point)
        return -1;

    return (sqrt(pow(_point->m_x, 2) + pow(_point->m_y, 2)));
}

double three_length(ThreeD *_point)
{
    if (!_point)
        return -1;

    return (sqrt(pow(_point->m_z, 2) + pow(_point->m_x, 2) + pow(_point->m_y, 2)));
}