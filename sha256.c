#include <stdio.h>
#include <string.h>
#include "sha256.h"

void sha256_init(sha256context ctx)
{
    ctx->bits = 0;
    memcpy(ctx->H, SHA256_INITIAL_HASH, sizeof(SHA256_INITIAL_HASH));
    memset(ctx->buf, 0, sizeof(ctx->buf));
}

void sha256_write(sha256context ctx, const char *data, size_t len)
{
    unsigned long bp = (ctx->bits >> 3) % SHA256_BLOCK_SIZE;
    unsigned long dp = 0;
    size_t tmp = len;
    while (tmp > 0)
    {
        if (bp + tmp < SHA256_BLOCK_SIZE)
        {
            memcpy(ctx->buf + bp, data + dp, tmp);
            ctx->bits += tmp << 3;
            break;
        }
        else
        {
            memcpy(ctx->buf + bp, data + dp, SHA256_BLOCK_SIZE - bp);
            dp += SHA256_BLOCK_SIZE - bp;
            tmp -= SHA256_BLOCK_SIZE - bp;
            ctx->bits += (SHA256_BLOCK_SIZE - bp) << 3;
            sha256_update(ctx);
            bp = 0;
        }
    }
}

void sha256_update(sha256context ctx)
{
    unsigned int W[SHA256_BLOCK_SIZE];
    for (int t = 0; t < SHA256_BLOCK_SIZE; t++)
    {
        if (t < 16)
        {
            W[t] = (ctx->buf[t * 4] << 24) + (ctx->buf[t * 4 + 1] << 16) +
                   (ctx->buf[t * 4 + 2] << 8) + (ctx->buf[t * 4 + 3]);
        }
        else
        {
            W[t] = sigma1(W[t - 2]) + W[t - 7] + sigma0(W[t - 15]) + W[t - 16];
        }
    }
    unsigned int a = ctx->H[0];
    unsigned int b = ctx->H[1];
    unsigned int c = ctx->H[2];
    unsigned int d = ctx->H[3];
    unsigned int e = ctx->H[4];
    unsigned int f = ctx->H[5];
    unsigned int g = ctx->H[6];
    unsigned int h = ctx->H[7];
    unsigned int T1, T2;
    for (int t = 0; t < SHA256_BLOCK_SIZE; t++)
    {
        T1 = h + SIGMA1(e) + Ch(e, f, g) + SHA256_K[t] + W[t];
        T2 = SIGMA0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }
    ctx->H[0] += a;
    ctx->H[1] += b;
    ctx->H[2] += c;
    ctx->H[3] += d;
    ctx->H[4] += e;
    ctx->H[5] += f;
    ctx->H[6] += g;
    ctx->H[7] += h;
    // sha256context_dump(ctx);
}

void sha256_finish(sha256context ctx, char *hash)
{
    unsigned long p = (ctx->bits >> 3) % SHA256_BLOCK_SIZE;
    unsigned int len = SHA256_BLOCK_SIZE - p;
    if (len < 9)
    {
        len += SHA256_BLOCK_SIZE;
    }
    unsigned char pad[len];
    memset(pad, 0, len);
    pad[0] = 0x80;
    pad[len - 8] = ctx->bits >> 56;
    pad[len - 7] = ctx->bits >> 48;
    pad[len - 6] = ctx->bits >> 40;
    pad[len - 5] = ctx->bits >> 32;
    pad[len - 4] = ctx->bits >> 24;
    pad[len - 3] = ctx->bits >> 16;
    pad[len - 2] = ctx->bits >> 8;
    pad[len - 1] = ctx->bits;
    sha256_write(ctx, pad, len);
    sprintf(hash, "%08x%08x%08x%08x%08x%08x%08x%08x",
            ctx->H[0], ctx->H[1], ctx->H[2], ctx->H[3],
            ctx->H[4], ctx->H[5], ctx->H[6], ctx->H[7]);
}

void hex2bs(unsigned char *bs, const char *hex)
{
    int len = strlen(hex);
    for (int i = 0; i < len; i += 2)
    {
        unsigned int x;
        sscanf((char *)(hex + i), "%02x", &x);
        bs[i / 2] = x;
    }
    return;
}

void sha256context_dump(sha256context ctx)
{
    printf("%lld %llx\n", ctx->bits, ctx->bits);
    for (int i = 0; i < SHA256_WORD_SIZE; i++)
    {
        printf("%08x", ctx->H[i]);
    }
    printf("\n");
    for (int i = 0; i < SHA256_BLOCK_SIZE; i++)
    {
        printf("%02x", ctx->buf[i]);
    }
    printf("\n");
}
