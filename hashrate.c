#define INC
#include "includes/includes.h"
#include "includes/sha_hashrate.h" 
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <omp.h>
void calculate_hashrate(uint8_t* msg, size_t msg_len, uint32_t max_nonce);

int main() {
    uint8_t msg[] = "Benchmarking SHA256";
    size_t msg_len = strlen((char*)msg);
    uint32_t max_nonce = UINT32_MAX;

    calculate_hashrate(msg, msg_len, max_nonce);
    return 0;
}

void calculate_hashrate_old(uint8_t* msg, size_t msg_len, uint32_t max_nonce) {
    struct SHA256_ctx ctx_base;
    struct SHA256_ctx ctx_extended;
    uint32_t hash_output[8] = {0};
    startTimer();
    v_sha_init(&ctx_base);
    v_sha_update(&ctx_base, msg, msg_len);
    
    uint8_t smsg[] = " secondmsg";

    uint8_t nonce_str[] ="aaaa";
    for(uint32_t nonce=0; nonce < max_nonce; nonce++) {
        // ctx_extended = ctx_base;
        copy_SHA256_ctx(&ctx_extended, &ctx_base);
        
        v_sha_update(&ctx_extended, nonce_str, 5);

        v_sha_finish(&ctx_extended, hash_output);
        
        // printf("a");

        if(hash_output[0] < 0x00FFFFFF)
        {
            // printf("\nHash: Zero %d %08x\n", (int)hash_output[0], hash_output[0]);
            v_printHex(hash_output, 8);
        }
        // (*(uint32_t*)nonce_str) +=10;
    }

    stopTimer();

    uint64_t mcs = getMicroseconds();

    printf("Hashes realizados: %u\n", max_nonce);
    printf("Tiempo transcurrido: %lu microsegundos\n", mcs);
    printf("Hashrate: %.2f hashes/segundo\n", (double)max_nonce / (mcs / 1000000.0));

}

void calculate_hashrate(uint8_t* msg, size_t msg_len, uint32_t max_nonce) {
    uint32_t hash_output[8] = {0};
    uint8_t* msg_nonce = (uint8_t*)malloc(msg_len + 5);
    memcpy(msg_nonce, msg, msg_len);
    *((uint32_t*)&msg_nonce[msg_len]) = 0x00000000;
    msg_nonce[msg_len + 4] = '\0';

    startTimer();

    // Paralelización del bucle con OpenMP
    #pragma omp parallel for private(hash_output, msg_nonce) shared(msg, msg_len) 
    for(uint32_t nonce=0; nonce < max_nonce; nonce++) {
        uint8_t* msg_nonce_local = (uint8_t*)malloc(msg_len + 5);
        memcpy(msg_nonce_local, msg, msg_len);
        *((uint32_t*)&msg_nonce_local[msg_len]) = nonce;
        msg_nonce_local[msg_len + 4] = '\0';

        // Llamada a la función de hash
        v_sha256_hash(msg_nonce_local, hash_output, msg_len + 5, false);

        if(hash_output[0] < 0x0000000F) {
            #pragma omp critical
            v_printHex(hash_output, 8); // Imprime de forma segura en paralelo
        }

        free(msg_nonce_local); // Liberar memoria asignada
    }

    free(msg_nonce); // Liberar la memoria original

    stopTimer();
    uint64_t mcs = getMicroseconds();

    // Impresión de resultados
    printf("Hashes realizados: %u\n", max_nonce);
    printf("Tiempo transcurrido: %lu microsegundos\n", mcs);
    printf("Hashrate: %.2f hashes/segundo\n", (double)max_nonce / (mcs / 1000000.0));
}
