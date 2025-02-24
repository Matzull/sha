#define INC
#include "includes/includes.h"
// #include "sha.h"
#include "includes/sha_hashrate.h" 
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


uint8_t* loadFile(char* filename, size_t* size)
{
    FILE* fd = fopen(filename, "rb");

    //Get file size in bytes
    fseek(fd, 0, SEEK_END);
    *size = ftell(fd);
    rewind(fd);
    long read;

    //Initialize msg
    char* msg = (char*)malloc(*size + 1);
    
    //Read file content
    if ((read = fread(msg, 1, *size, fd)) != *size)
    {
        printf("Failed to read file, Real size: %ld, Read size: %ld", *size, read);
        exit(0);
    }
    msg[*size] = '\0';
    fclose(fd);
    return msg;
}

int main(int argc, char* argv[]) {
    int option;
    uint32_t digest[8];
    uint8_t* msg;
    bool viz = false;
    size_t size;
    while ((option = getopt(argc, argv, "hbs:f:")) != -1) {
        switch (option) {
        case 'h':
            printf("-h show this help\n");
            printf("-s hash from argument\n");
            printf("-b graphic mode\n");
            printf("-f hash from file\n");
            return 0;
        case 'f':
            printf("Loading from file\n");
            msg = (uint8_t*)loadFile((char*)optarg, &size);
            printf("Loaded\n");
            startTimer();
            v_sha256_hash(msg, digest, size, viz);
            stopTimer();
            v_print_sha256_hash(digest, size);
            free(msg);
            break;
        case 's':
            msg = (uint8_t*)optarg;
            v_sha256_hash(msg, digest, strlen((const char*)msg), viz);
            v_print_sha256_hash(digest, strlen((const char*)msg));
            free(msg);   
            break;
        case 'b':
            viz = true;
            break;
        default:
            printf("Opción inválida. Use -h para obtener ayuda.\n");
            return 1;
        }
    }
}