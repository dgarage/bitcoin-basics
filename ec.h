
#ifndef EC_H
#define EC_H

#include <gmp.h>

typedef struct
{
    mpz_t X;
    mpz_t Y;
} point_t[1];

void point_init(point_t);

void point_add(point_t, const point_t, const point_t);

void point_mul(point_t, const mpz_t, const point_t);

#endif