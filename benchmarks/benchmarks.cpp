#include <iostream>
#include <ctime>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <functional>
#include <map>

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
    bool Matrix4Transpose();
    bool dot();
}

bool Benchmarks::Regressed = true;

static std::map<std::string, std::pair<bool, std::function<bool()>>> benchmarks = {
    { "dot",              { false, Check::dot              }},
    { "Matrix4Transpose", { false, Check::Matrix4Transpose }}
};

}

int main()
{
    int sum  = 0;
    int fail = 0;
    for (auto &b : Test::benchmarks)
    {
        sum++;
        auto &[name,  props] = b;
        auto &[passed, func] = props;

        passed = func();
        if (!passed)
        {
            fail++;
        }
    }

    for (auto &b : Test::benchmarks)
    {
        auto &[name,  props] = b;
        auto &[passed, func] = props;

        if (passed)
        {
            printf("\033[1;33mTest: %s\033[0m\t=> %s", name.c_str(), "[ \033[0;32;32mOK\033[0m  ]");
        }
        else
        {

            printf("\033[1;33mTest: %s\033[0m\t=> %s", name.c_str(), "[ \033[1;31;40mFail\033[0m ]");
        }
    }

    printf("\033[1;36mTest: passed %d/%d\033[0m", (sum - fail), sum);
    return 0;

    return 0;
}