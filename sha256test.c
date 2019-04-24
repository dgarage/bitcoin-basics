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
    hextobs(data, hex);
    sha256_write(ctx, data, len);
    char hash[32];
    sha256_finish(ctx, hash);
    char hstr[64];
    sprint_h(hstr, hash, 32);
    int ret = strcmp(hstr, value);
    if (ret != 0)
    {
        printf("%d %s %s\n", ret, hash, value);
    }
    return ret;
}

int main()
{
    int size, ret;
    size = sizeof(TEST_CASE_HMAC_SHA256) / sizeof(TEST_CASE_HMAC_SHA256[0]);
    for (int i = 0; i < size; i += 3)
    {
        unsigned char hmac[32];
        char *Key = TEST_CASE_HMAC_SHA256[i];
        char *Data = TEST_CASE_HMAC_SHA256[i + 1];
        char *HMAC = TEST_CASE_HMAC_SHA256[i + 2];
        unsigned char key[strlen(Key) / 2];
        unsigned char data[strlen(Data) / 2];
        hextobs(key, Key);
        hextobs(data, Data);
        hmac_sha256(hmac, key, strlen(Key) / 2, data, strlen(Data) / 2);
        unsigned char hmachex[64];
        sprint_h(hmachex, hmac, 32);
        ret = strcmp(hmachex, HMAC);
        if (ret != 0)
        {
            printf("%d %s\n", i, TEST_CASE_HMAC_SHA256[i]);
            break;
        }
    }
    if (ret != 0)
    {
        printf("TEST_CASE_HMAC_SHA256 Fail.\n");
        return -1;
    }
    printf("TEST_CASE_HMAC_SHA256 Complete!\n");
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
    printf("TEST_SHA256_SHORT     Complete!\n");
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
    printf("TEST_SHA256_LONG      Complete!\n");
    return 0;
}
