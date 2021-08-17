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
            duration = ((float)end - (float)start) / CLOCKS_PER_SEC;
            printf("\nTime Consuming: %g\n", duration);
        }

    private:
        clock_t start;
        clock_t end;
        float duration;
    };
}
