#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include "ec.h"

int main()
{
    mpz_t N;
    mpz_init_set_str(N, "fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141", 16);
    point G = newPoint();
    mpz_init_set_str(G.X, "79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798", 16);
    mpz_init_set_str(G.Y, "483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8", 16);
    int size = 1000;
    mpz_t ks[size];
    gmp_randstate_t state;
    gmp_randinit_default(state);
    time_t rtime;
    time(&rtime);
    gmp_randseed_ui(state, rtime);
    for (int i = 0; i < size; i++)
    {
        mpz_init(ks[i]);
        mpz_urandomm(ks[i], state, N);
    }
    clock_t s, e;
    printf("loop %d times\n", size);
    s = clock();
    for (int i = 0; i < size; i++)
    {
        mulPoint(ks[i], G);
    }
    e = clock();
    printf("finish %.5f sec\n", (double)(e - s) / CLOCKS_PER_SEC);
}