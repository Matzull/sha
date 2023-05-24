#define INC
#include "includes.h"
#include "sha.h"
#include "sha_viz.h" 


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
        printf("Failed to read file, Real size: %d, Read size: %d", *size, read);
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
            printf("-b visualitation mode\n");
            printf("-f hash from file\n");
            return 0;
        case 'f':
            printf("Loading from file\n");
            msg = (uint8_t*)loadFile((char*)optarg, &size);
            printf("Loaded\n");
            if (viz)
            {
                v_sha256_hash(msg, digest, size);
                v_print_sha256_hash(digest, size);
            }
            else
            {
                sha256_hash(msg, digest, size);
                print_sha256_hash(digest, size);
            }
            break;
        case 's':
            msg = (uint8_t*)optarg;
            if (viz)
            {
                v_sha256_hash(msg, digest, strlen((const char*)msg));
                v_print_sha256_hash(digest, strlen((const char*)msg));
            }
            else
            {
                sha256_hash(msg, digest, strlen((const char*)msg));
                print_sha256_hash(digest, strlen((const char*)msg));
            }         
            break;
        case 'b':
            viz = true;
            break;
        default:
            printf("Opción inválida. Use -h para obtener ayuda.\n");
            return 1;
        }
    }
    free(msg);
}