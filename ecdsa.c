#include <gmp.h>
#include <string.h>
#include "sha256.h"
#include "ecdsa.h"
#include "ec.h"

void sign(mpz_t s, mpz_t r, const mpz_t x, const unsigned char *m)
{
}

int verify(const mpz_t s, const mpz_t r, const point_t p, const unsigned char *m)
{
}

void nonceRFC6979(mpz_t k, const mpz_t x, const unsigned char *m)
{
    mpz_t N;
    mpz_init_set_str(N, "fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141", 16);
    sha256context ctx;
    // https://tools.ietf.org/html/rfc6979#section-3.2
    // a.  Process m through the hash function H, yielding:
    //        h1 = H(m)
    unsigned char h1[32];
    sha256_init(ctx);
    sha256_write(ctx, m, strlen(m));
    sha256_finish(ctx, h1);
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
