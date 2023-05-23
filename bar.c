#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "loading_bar.h"

int main()
{
    init_bar(100);
    for (size_t i = 0; i < 100; i++)
    {
        usleep(50000);
        update_bar();
        print_bar();
    }
}