#include <stdio.h>
#include <time.h>
#include <string.h>
#include "sha256.h"
#include "ecdsa.h"
#include "ec.h"

int testNonceRFC6979()
{
    char *TEST_ECDSA[] = {
        "sample",
        "A6E3C57DD01ABE90086538398355DD4C3B17AA873382B0F24D6129493D8AAD60",
        "test",
        "D16B6AE827F17175E040871A1C7EC3500192C4C92677336EC2537ACAEE0008E0",
    };
    for (int i = 0; i < 4; i += 2)
    {
        mpz_t x;
        mpz_init(x);
        mpz_init_set_str(x, "C9AFA9D845BA75166B5C215767B1D6934E50C3DB36E89B127B8A622B120F6721", 16);
        char *m = TEST_ECDSA[i];
        char *K = TEST_ECDSA[i + 1];
        mpz_t k;
        mpz_init(k);
        unsigned char h[32];
        sha256context ctx;
        sha256_init(ctx);
        sha256_write(ctx, m, strlen(m));
        sha256_finish(ctx, h);
        nonceRFC6979(k, x, h);
        char str[64];
        gmp_sprintf(str, "%064ZX", k);
        int ret = strcmp(str, K);
        if (ret != 0)
        {
            gmp_printf("%064ZX %s\n", k, K);
            return ret;
        }
    }
    return 0;
}

int main()
{
    int ret = testNonceRFC6979();
    if (ret != 0)
    {
        printf("Test nonceRFC6979 Fail!\n");
    }
    printf("Test nonceRFC6979 Complete!\n");
}