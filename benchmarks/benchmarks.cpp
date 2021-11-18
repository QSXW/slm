#include <iostream>
#include <ctime>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <functional>

#include "benchmarks.h"

namespace Test
{

bool CompareFloatingPoint(float a, float b, float epsilon)
{
    float diff = fabsf(a - b);

    if (diff < epsilon)
    {
        return true;
    }
    Fail( "Test failed comparing %g with %g (abs diff=%g with epsilon=%g)\n", a, b, diff, epsilon);

    return false;
}

namespace Check
{
    void Matrix4Transpose();
    void dot();
}

bool Benchmarks::Regressed = true;

static const std::vector<std::pair<const char *, std::function<void()>>> benchmarks = {
    { "dot",              Check::dot              },
    { "Matrix4Transpose", Check::Matrix4Transpose }
};

}

int main()
{
    for (const auto &b : Test::benchmarks)
    {
        printf("\n[%s]:\n", b.first);
        b.second();
    }

    return 0;
}