#include <time.h>
struct timespec start, end;
long elapsed_ms;

void startTimer()
{
    clock_gettime(CLOCK_MONOTONIC, &start);
}

void stopTimer()
{
    clock_gettime(CLOCK_MONOTONIC, &end);
}

long getMs()
{
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
}