#ifndef INC
#define INC
#include "includes.h"
#endif
#include "sha_viz.h"



#define VIZ
// #define VIZ_EXTRA
#define H1 0x6a09e667
#define H2 0xbb67ae85
#define H3 0x3c6ef372
#define H4 0xa54ff53a
#define H5 0x510e527f
#define H6 0x9b05688c
#define H7 0x1f83d9ab
#define H8 0x5be0cd19

#define XOR(x, y) x ^ y
#define AND(x, y) x &y
#define OR(x, y) x | y
#define CMP(x) ~x
#define SM(x, y) x + y
#define RSHIFT(x, n) x >> n
#define RROT(x, n) ((x >> n) | (x << (32 - n)))

#define CH(x, y, z) XOR((AND(x, y)), (AND((CMP(x)), z)))
#define MAJ(x, y, z) XOR((AND(x, y)), XOR((AND(x, z)), (AND(y, z))))
#define SIG0(x) XOR((RROT(x, 2)), XOR((RROT(x, 13)), (RROT(x, 22))))
#define SIG1(x) XOR((RROT(x, 6)), XOR((RROT(x, 11)), (RROT(x, 25))))
#define PHI0(x) XOR((RROT(x, 7)), XOR((RROT(x, 18)), (RSHIFT(x, 3))))
#define PHI1(x) XOR((RROT(x, 17)), XOR((RROT(x, 19)), (RSHIFT(x, 10))))

typedef struct SHA256_ctx
{
    uint32_t Hi[8];
    uint32_t w[64];
    int current_block;
    uint8_t* msg;
    uint32_t blocks;
};

int copy_SHA256_ctx(struct SHA256_ctx* dest, const struct SHA256_ctx* src) {
    memcpy(dest->Hi, src->Hi, 8);
    memcpy(dest->w, src->w, 64);
    dest->current_block = 0;
    dest->blocks = 0;
    return 0;
}

const uint32_t v_k[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

// len in bits
uint8_t* v_pad(uint8_t* msg, uint64_t len, uint32_t* blocks)
{
    uint8_t *ptr;
    uint32_t zeroes = 448 - ((len + 1) % 448);
    uint64_t p_msg_len = (len + 1 + zeroes + 64);
    *blocks = p_msg_len / 512;
    uint8_t* paddedmsg = (uint8_t*)malloc(p_msg_len>>3);
    if (!paddedmsg)
    {
        printf("Error allocating memory\n");
        exit(1);
    }
    memset(paddedmsg, 0x00, (*blocks) * 64);
    memcpy(paddedmsg, msg, (len >> 3)); // original message
    ptr = paddedmsg + (len >> 3);
    ptr[0] = 0x80; // 1000 0000 -> single bit that is set to 1 at the end of the message
    len = bswap_64 (len);
    memcpy((paddedmsg + (*blocks) * 64) - 8, &len, 8); // length
    return paddedmsg;
}

void v_sha_init(struct SHA256_ctx* ctx)
{
    ctx->current_block = 0;
    ctx->Hi[0] = H1;
    ctx->Hi[1] = H2;
    ctx->Hi[2] = H3;
    ctx->Hi[3] = H4;
    ctx->Hi[4] = H5;
    ctx->Hi[5] = H6;
    ctx->Hi[6] = H7;
    ctx->Hi[7] = H8;
}

void v_sha_process_block(struct SHA256_ctx* ctx)
{
    uint32_t a, b, c, d, e, f, g, h;
    a = ctx->Hi[0];
    b = ctx->Hi[1];
    c = ctx->Hi[2];
    d = ctx->Hi[3];
    e = ctx->Hi[4];
    f = ctx->Hi[5];
    g = ctx->Hi[6];
    h = ctx->Hi[7];

    for (int j = 0; j < 64; j++)
    {
        #ifdef VIZ_EXTRA
        v_clearScreen(100000);
        v_printMemory(ctx->Hi,8);
        printf("Current block: %d/%d\n", ctx->current_block, ctx->blocks);
        v_print_values(j,a,b,c,d,e,f,g,h);
        print_bar();    
        #endif
        uint32_t ch = CH(e, f, g);
        uint32_t maj = MAJ(a, b, c);
        uint32_t sig_0 = SIG0(a);
        uint32_t sig_1 = SIG1(e);

        // printf("%s %d %d %d\n", ctx->msg, ctx->current_block, j, ctx->w[j]);
        if (j < 16)
        {
            ctx->w[j] = __builtin_bswap32(*((uint32_t *)(ctx->msg + (ctx->current_block * 64) + (j * 4))));
        }
        else
        {
            ctx->w[j] = (PHI1(ctx->w[j - 2])) + ctx->w[j - 7] + (PHI0(ctx->w[j - 15])) + ctx->w[j - 16];
        }

        uint32_t t1 = h + sig_1 + ch + v_k[j] + ctx->w[j];
        uint32_t t2 = sig_0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    ctx->Hi[0] += a;
    ctx->Hi[1] += b;
    ctx->Hi[2] += c;
    ctx->Hi[3] += d;
    ctx->Hi[4] += e;
    ctx->Hi[5] += f;
    ctx->Hi[6] += g;
    ctx->Hi[7] += h;
    ctx->current_block++;
}

void v_sha_finish(struct SHA256_ctx* ctx, uint32_t* digest)
{
    digest[0] = ctx->Hi[0];
    digest[1] = ctx->Hi[1];
    digest[2] = ctx->Hi[2];
    digest[3] = ctx->Hi[3];
    digest[4] = ctx->Hi[4];
    digest[5] = ctx->Hi[5];
    digest[6] = ctx->Hi[6];
    digest[7] = ctx->Hi[7];
}

void v_sha_update(struct SHA256_ctx* ctx, uint8_t* msg, size_t len)
{
    ctx->msg = v_pad(msg, len << 3, &ctx->blocks);
    #ifdef VIZ
    init_bar(ctx->blocks);
    #endif
    for (int i = 0; i < ctx->blocks; i++)
    {
        v_sha_process_block(ctx);
        #ifdef VIZ
        update_bar();
        print_bar_s();
        #endif
    }
    free(ctx->msg);
}

void v_hash(uint8_t* msg, size_t len, uint32_t* digest, bool viz)
{
    struct SHA256_ctx ctx;
    
    v_sha_init(&ctx);
    v_sha_update(&ctx, msg, len);
    v_sha_finish(&ctx, digest);
}

void v_sha256_hash(uint8_t *msg, uint32_t* digest, size_t len, bool viz)
{
    // startTimer();
    v_hash(msg, len, digest, viz);
    // stopTimer();
}

void v_print_sha256_hash(uint32_t* digest, size_t len)
{
    printf("\n");
    printf("Hash computed in: %d ms\n", (int)(getMicroseconds()/1000.0));
    printf("Hashing speed: %d MB/s\n", (int)((len/1000000.0)/(getMicroseconds()/1000000.0)));
    v_printHex(digest, 8);
    printf("\n");
}