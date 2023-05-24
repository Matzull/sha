#include <time.h>
#include <stdint.h>
struct timespec start, end;
uint64_t elapsed_ms;

void startTimer()
{
    clock_gettime(CLOCK_MONOTONIC, &start);
}

void stopTimer()
{
    clock_gettime(CLOCK_MONOTONIC, &end);
}

uint64_t getMs()
{
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
}