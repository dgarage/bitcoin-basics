
#ifndef EC_H
#define EC_H

#include <gmp.h>

typedef struct
{
    mpz_t X;
    mpz_t Y;
} point;

point newPoint();

point addPoint(point p1, point p2);

point mulPoint(mpz_t k, point p);

#endif