#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <immintrin.h>  // For endianness conversion
#include <unistd.h>
#include "sha.h"
#include "sha_viz.h"

int main(int argc, char* argv[]) {
    int option;
    uint32_t digest[8];
    while ((option = getopt(argc, argv, "hs:f:b:")) != -1) {
        switch (option) {
        case 'h':
            printf("-h show this help\n");
            printf("-s hash from argument\n");
            printf("-b hash from argument\n");
            printf("-f hash from file\n");
            return 0;
        case 'f':
            printf("Loading from file\n");
            break;
        case 's':
            sha256_hash((uint8_t*)optarg, digest);
            print_sha256_hash(digest);
            break;
        case 'b':
            v_sha256_hash((uint8_t*)optarg, digest);
            v_print_sha256_hash(digest);
            break;
        default:
            printf("Opción inválida. Use -h para obtener ayuda.\n");
            return 1;
        }
    }
}