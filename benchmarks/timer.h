#pragma once
#include <cstdio>
#include <ctime>

namespace slm
{

class Timer
{
public:
    Timer()
    {
        start = clock();
    }

    ~Timer()
    {
        end = clock();
        clock_t count = end - start;
        double duration = (double)count / CLOCKS_PER_SEC;
        printf("\nClock: %d\tTime Consuming: %g\n", (int)count, duration);
    }

private:
    clock_t start;
    clock_t end;
};

}
