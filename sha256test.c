#include <stdio.h>
#include <string.h>
#include "sha256.h"
#include "sha256test.h"

int test(char *hex, char *value)
{
    sha256context ctx;
    sha256_init(ctx);
    size_t len = strlen(hex) / 2;
    unsigned char data[len];
    hex2bs(data, hex);
    sha256_write(ctx, data, len);
    char hash[64];
    sha256_finish(ctx, hash);
    int ret = strcmp(hash, value);
    if (ret != 0)
    {
        printf("%d %s %s\n", ret, hash, value);
    }
    return ret;
}

int main()
{
    int size, ret;
    size = sizeof(TEST_SHA256_SHORT) / sizeof(TEST_SHA256_SHORT[0]);
    for (int i = 0; i < size; i += 2)
    {
        ret = test(TEST_SHA256_SHORT[i], TEST_SHA256_SHORT[i + 1]);
        if (ret != 0)
        {
            printf("%d %s\n", i, TEST_SHA256_SHORT[i]);
            break;
        }
    }
    if (ret != 0)
    {
        printf("TEST_SHA256_SHORT Fail.\n");
        return -1;
    }
    printf("TEST_SHA256_SHORT Complete!\n");
    size = sizeof(TEST_SHA256_LONG) / sizeof(TEST_SHA256_LONG[0]);
    for (int i = 0; i < size; i += 2)
    {
        ret = test(TEST_SHA256_LONG[i], TEST_SHA256_LONG[i + 1]);
        if (ret != 0)
        {
            printf("%d %s\n", i, TEST_SHA256_LONG[i]);
            break;
        }
    }
    if (ret != 0)
    {
        printf("TEST_SHA256_LONG Fail.\n");
        return -1;
    }
    printf("TEST_SHA256_LONG Complete!\n");
    return 0;
}
