
#include <gmp.h>
#include "ec.h"

point newPoint()
{
    point p;
    mpz_init(p.X);
    mpz_init(p.Y);
    return p;
}

point addPoint(point p1, point p2)
{
    point p3 = newPoint();
    mpz_t ZERO;
    mpz_init(ZERO);
    mpz_t THREE;
    mpz_init_set_ui(THREE, 3);
    mpz_t P;
    mpz_init_set_str(P, "fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f", 16);
    if (mpz_cmp(p1.X, ZERO) == 0 || mpz_cmp(p1.Y, ZERO) == 0)
    {
        mpz_set(p3.X, p2.X);
        mpz_set(p3.Y, p2.Y);
        return p3;
    }
    if (mpz_cmp(p2.X, ZERO) == 0 || mpz_cmp(p2.Y, ZERO) == 0)
    {
        mpz_set(p3.X, p1.X);
        mpz_set(p3.Y, p1.Y);
        return p3;
    }
    if (mpz_cmp(p1.X, p2.X) == 0)
    {
        if (mpz_cmp(p1.Y, p2.Y) == 0)
        {
            mpz_t lam;
            mpz_init(lam);
            mpz_powm_ui(lam, p1.X, 2, P); // lam = x^2 mod P
            mpz_mul_ui(lam, lam, 3);      // lam = 3*x^2
            mpz_t tmp;                    //
            mpz_init_set_ui(tmp, 2);      // tmp = 2
            mpz_mul(tmp, tmp, p1.Y);      // tmp = 2 * y1
            mpz_invert(tmp, tmp, P);      // tmp = 1 / (2 * y1)
            mpz_mul(lam, lam, tmp);       // lam = (3 * x1^2) / (2 * y1)
            mpz_mod(lam, lam, P);         // lam = (3 * x1^2) / (2 * y1) mod P
            mpz_powm_ui(p3.X, lam, 2, P); // x3 = lam^2
            mpz_submul_ui(p3.X, p1.X, 2); // x3 = lam^2 - 2 * x1
            mpz_mod(p3.X, p3.X, P);       // x3 = x3 mod P
            mpz_sub(tmp, p1.X, p3.X);     // tmp = x1 - x3
            mpz_mul(tmp, lam, tmp);       // tmp = lam * (x1 - x3)
            mpz_sub(p3.Y, tmp, p1.Y);     // y3 = lam * (x1 - x3) - y1
            mpz_mod(p3.Y, p3.Y, P);       // y3 = y3 mod P
            return p3;
        }
        return p3;
    }
    mpz_t lam;
    mpz_init(lam);
    mpz_sub(lam, p2.Y, p1.Y);     // lam = y2 - y1
    mpz_t tmp;                    //
    mpz_init(tmp);                //
    mpz_sub(tmp, p2.X, p1.X);     // tmp = x2 - x1
    mpz_invert(tmp, tmp, P);      // tmp = 1 / (x2 - x1)
    mpz_mul(lam, lam, tmp);       // lam = (y2 - y1) / (x2 - x1)
    mpz_mod(lam, lam, P);         // lam = lam mod P
    mpz_powm_ui(p3.X, lam, 2, P); // x3 = lam^2
    mpz_sub(p3.X, p3.X, p1.X);    // x3 = lam^2 - x1
    mpz_sub(p3.X, p3.X, p2.X);    // x3 = lam^2 - x1 - x2
    mpz_mod(p3.X, p3.X, P);       // x3 = x3 mod P
    mpz_sub(tmp, p1.X, p3.X);     // tmp = x1 - x3
    mpz_mul(tmp, lam, tmp);       // tmp = lam * (x1 - x3)
    mpz_sub(p3.Y, tmp, p1.Y);     // y3 = lam * (x1 - x3) - y1
    mpz_mod(p3.Y, p3.Y, P);       // y3 = y3 mod P
    return p3;
}

point mulPoint(mpz_t k, point p)
{
    point kp = newPoint();
    mpz_t pos;
    mpz_init_set_ui(pos, 1);
    mpz_t a;
    mpz_init(a);
    point tmp = newPoint();
    mpz_set(tmp.X, p.X);
    mpz_set(tmp.Y, p.Y);
    while (mpz_cmp(pos, k) <= 0)
    {
        mpz_and(a, k, pos);
        if (mpz_cmp_ui(a, 0) > 0)
        {
            kp = addPoint(kp, tmp);
        }
        tmp = addPoint(tmp, tmp);
        mpz_mul_2exp(pos, pos, 1);
    }
    return kp;
}