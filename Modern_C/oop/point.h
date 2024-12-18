#ifndef __POINT__H
#define __POINT__H

struct TwoD
{
    int m_x;
    int m_y;

    // Not declare function pointer in struct , case it will increase space.
    // That also the CPP virtual table implement details.
};
typedef struct TwoD TwoD;

double              length(TwoD *_point);

struct ThreeD
{
    // Like the inheritance, Using Anonymous and UnAnonymous object to more convenient usage.
    union
    {
        struct TwoD;  // Anonymous struct member, need C11 and compiler flag `-fms_extensions`
        TwoD two_d;
    };

    int m_z;
};

typedef struct ThreeD ThreeD;

double                three_length(ThreeD *_point);

#endif