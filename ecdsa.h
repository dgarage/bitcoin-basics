#ifndef ECDSA_H
#define ECDSA_H

#include <gmp.h>
#include "ec.h"

void sign(mpz_t, mpz_t, const mpz_t, const unsigned char *);

int verify(const mpz_t, const mpz_t, const point_t, const unsigned char *);

void nonceRFC6979(mpz_t, const mpz_t, const unsigned char *);

#endif