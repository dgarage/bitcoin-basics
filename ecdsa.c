#include <gmp.h>
#include <string.h>
#include "sha256.h"
#include "ecdsa.h"
#include "ec.h"

void sign(mpz_t s, mpz_t r, const mpz_t x, const unsigned char *h)
{
    point_t G;
    point_init(G);
    mpz_init_set_str(G->X, "79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798", 16);
    mpz_init_set_str(G->Y, "483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8", 16);
    mpz_t k;
    mpz_init(k);
    nonceRFC6979(k, x, h);
    point_t R;
    point_init(R);
    point_mul(R, k, G);
    mpz_set(r, R->X);
    mpz_t N;
    mpz_init_set_str(N, "fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141", 16);
    mpz_invert(k, k, N);
    mpz_mul(s, r, x);
    char hex[64];
    sprint_h(hex, h, 32);
    mpz_t e;
    mpz_set_str(e, hex, 16);
    mpz_add(s, s, e);
    mpz_mul(s, s, k);
    mpz_mod(s, s, N);
    mpz_t half;
    mpz_init_set_str(half, "7fffffffffffffffffffffffffffffff5d576e7357a4501ddfe92f46681b20a0", 16);
    if (mpz_cmp(s, half) > 0)
    {
        mpz_sub(s, N, s);
    }
}

int verify(const mpz_t s, const mpz_t r, const point_t P, const unsigned char *h)
{
    mpz_t N;
    mpz_init_set_str(N, "fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141", 16);
    mpz_t w;
    mpz_init(w);
    mpz_invert(w, s, N);
    char hex[64];
    sprint_h(hex, h, 32);
    mpz_t e;
    mpz_set_str(e, hex, 16);
    mpz_t u1;
    mpz_init(u1);
    mpz_mul(u1, e, w);
    mpz_mod(u1, u1, N);
    mpz_t u2;
    mpz_init(u2);
    mpz_mul(u2, r, w);
    mpz_mod(u2, u2, N);
    point_t p1;
    point_init(p1);
    point_t G;
    point_init(G);
    mpz_init_set_str(G->X, "79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798", 16);
    mpz_init_set_str(G->Y, "483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8", 16);
    point_mul(p1, u1, G);
    point_t p2;
    point_init(p2);
    point_mul(p2, u2, P);
    point_t Q;
    point_init(Q);
    point_add(Q, p1, p2);
    return mpz_cmp(r, Q->X);
}

void nonceRFC6979(mpz_t k, const mpz_t x, const unsigned char *h)
{
    mpz_t N;
    mpz_init_set_str(N, "fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141", 16);
    sha256context ctx;
    // https://tools.ietf.org/html/rfc6979#section-3.2
    // a.  Process m through the hash function H, yielding:
    //        h1 = H(m)
    // パラメータがハッシュ値なので、コピーのみ
    unsigned char h1[32];
    memcpy(h1, h, 32);
    // b.  Set:
    //       V = 0x01 0x01 0x01 ... 0x01
    unsigned char V[32];
    memset(V, 1, 32);
    // c.  Set:
    //       K = 0x00 0x00 0x00 ... 0x00
    unsigned char K[32];
    memset(K, 0, 32);
    // d.  Set:
    //       K = HMAC_K(V || 0x00 || int2octets(x) || bits2octets(h1))
    unsigned char int2octets_x[32];
    // unsigned char bits2octets_h1[32];
    char str[64];
    gmp_sprintf(str, "%064Zx", x);
    hextobs(int2octets_x, str);
    unsigned char data[32 + 1 + 32 + 32];
    memcpy(data, V, 32);
    data[32] = 0;
    memcpy(data + (32 + 1), int2octets_x, 32);
    memcpy(data + (32 + 1 + 32), h1, 32);
    hmac_sha256(K, K, 32, data, 32 + 1 + 32 + 32);
    // e.  Set:
    //       V = HMAC_K(V)
    hmac_sha256(V, K, 32, V, 32);
    // f.  Set:
    //       K = HMAC_K(V || 0x01 || int2octets(x) || bits2octets(h1))
    memcpy(data, V, 32);
    data[32] = 1;
    hmac_sha256(K, K, 32, data, 32 + 1 + 32 + 32);
    // g.  Set:
    //       V = HMAC_K(V)
    hmac_sha256(V, K, 32, V, 32);
    // h.  Apply the following algorithm until a proper value is found for k:
    while (1)
    {
        // 1.  Set T to the empty sequence.
        unsigned char T[32];
        memset(T, 0, 32);
        // 2.  While tlen < qlen, do the following:
        //        V = HMAC_K(V)
        //        T = T || V
        hmac_sha256(V, K, 32, V, 32);
        memcpy(T, V, 32);
        // 3.  Compute:
        //        k = bits2int(T)
        sprint_h(str, T, 32);
        mpz_set_str(k, str, 16);
        // If that value of k is within the [1,q-1] range, and is
        // suitable for DSA or ECDSA (i.e., it results in an r value
        // that is not 0; see Section 3.4), then the generation of k is
        // finished.  The obtained value of k is used in DSA or ECDSA.
        // Otherwise, compute:
        if (mpz_cmp_ui(k, 0) > 0 && mpz_cmp(k, N) < 0)
        {
            break;
        }
        //     K = HMAC_K(V || 0x00)
        //     V = HMAC_K(V)
        unsigned char tmp[33];
        memcpy(tmp, V, 32);
        tmp[32] = 0;
        hmac_sha256(K, K, 32, tmp, 33);
        hmac_sha256(V, K, 32, V, 32);
    }
}
