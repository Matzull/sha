#include <stdio.h>



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
    printf("It = %d\na = %08x b = %08x c = %08x d = %08x e = %08x f = %08x g = %08x h = %08x\n", it, a, b, c, d, e, f, g, h);
}

void v_clearScreen(int usleepTime)
{
    usleep(usleepTime);
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