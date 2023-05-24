#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <byteswap.h>
#include <unistd.h>
#include <immintrin.h>
#include <time.h>
// #include "loading_bar.h"

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

const uint32_t v_k[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void v_printBinary(uint8_t *ptr, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        uint8_t value = ptr[i];
        for (int j = 7; j >= 0; j--)
        {
            printf("%d", (value >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

void v_printMemory(uint32_t *ptr, size_t length)
{
    uint8_t *bytePtr = (uint8_t *)ptr; // Se interpreta el puntero como un puntero a uint8_t

    for (size_t i = 0; i < length * sizeof(uint32_t); i++)
    {
        if (i % 4 == 0)
        {
            printf("\n");
        }
        uint8_t value = bytePtr[i];
        for (int j = 7; j >= 0; j--)
        {
            printf("%d", (value >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

void v_print_values(int it, int a, int b, int c, int d, int e, int f, int g, int h)
{
    printf("It = %d a = %08x b = %08x c = %08x d = %08x e = %08x f = %08x g = %08x h = %08x\n", it, a, b, c, d, e, f, g, h);
}

void v_clearScreen()
{
    // usleep(50);
    printf("\033[2J");
    printf("\033[0;0H");
    fflush(stdout);
}


void v_printHex(uint32_t *array, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        printf("%08x ", array[i]);
    }
    printf("\n");
}

// len in bits
uint8_t* v__pad(uint8_t* msg, uint8_t* paddedmsg, uint64_t len, uint32_t* blocks)
{
    uint8_t *ptr;
    uint32_t zeroes = 448 - ((len + 1) % 448);
    uint64_t p_msg_len = (len + 1 + zeroes + 64);
    *blocks = p_msg_len / 512;
    paddedmsg = (uint8_t*)malloc(p_msg_len>>3);
    memset(paddedmsg, 0x00, (*blocks) * 64);
    memcpy(paddedmsg, msg, (len >> 3)); // original message
    ptr = paddedmsg + (len >> 3);
    ptr[0] = 0x80; // 1000 0000
    len = bswap_64 (len);
    memcpy((paddedmsg + (*blocks) * 64) - 8, &len, 8); // length
    return paddedmsg;
}

void v__hash(uint32_t blocks, uint8_t* paddedmsg, uint32_t* digest)
{
    init_bar((int)blocks);
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t Hi[8] = {H1, H2, H3, H4, H5, H6, H7, H8};
    uint32_t w[64];
    for (size_t i = 0; i < blocks; i++)
    {
        a = Hi[0];
        b = Hi[1];
        c = Hi[2];
        d = Hi[3];
        e = Hi[4];
        f = Hi[5];
        g = Hi[6];
        h = Hi[7];
        update_bar();
        for (size_t j = 0; j < 64; j++)
        {
            v_clearScreen();
            v_printMemory(Hi,8);
            v_print_values(j, a,b,c,d,e,f,g,h);
            print_bar();
            uint32_t ch = CH(e, f, g);
            uint32_t maj = MAJ(a, b, c);
            uint32_t sig_0 = SIG0(a);
            uint32_t sig_1 = SIG1(e);

            if (j < 16)
            {
                w[j] = __builtin_bswap32(*((uint32_t *)(paddedmsg + (i * 64) + (j * 4))));
            }
            else
            {
                w[j] = (PHI1(w[j - 2])) + w[j - 7] + (PHI0(w[j - 15])) + w[j - 16];
            }

            uint32_t t1 = h + sig_1 + ch + v_k[j] + w[j];
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
        Hi[0] += a;
        Hi[1] += b;
        Hi[2] += c;
        Hi[3] += d;
        Hi[4] += e;
        Hi[5] += f;
        Hi[6] += g;
        Hi[7] += h;
    }
    digest[0] = Hi[0];
    digest[1] = Hi[1];
    digest[2] = Hi[2];
    digest[3] = Hi[3];
    digest[4] = Hi[4];
    digest[5] = Hi[5];
    digest[6] = Hi[6];
    digest[7] = Hi[7];
}

void v_sha256_hash(uint8_t *msg, uint32_t* digest, size_t len)
{
    uint8_t *padded = 0;
    uint32_t blocks;
    padded = v__pad(msg, padded, len << 3, &blocks);
    v__hash(blocks, padded, digest);
    free(padded);
}

void v_print_sha256_hash(uint32_t* digest)
{
    v_printHex(digest, 8);
    printf("\n");
}