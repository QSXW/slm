#include <iostream>
#include <ctime>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <functional>

#include "benchmarks.h"
#include "vector.h"
#include "matrix.h"

namespace Test
{

static Matrix4 matrix4;
static Vector4 vector4;

}

int main()
{
    std::vector<const Test::Unit *> benchmarks;
    benchmarks.emplace_back(&Test::matrix4);
    benchmarks.emplace_back(&Test::vector4);

    int sum  = 0;
    int fail = 0;
    for (auto &unit : benchmarks)
    {
        sum++;
        auto &name = unit->name();

        if (unit->comformance())
        {
            printf("\033[1;33mTest: %s\033[0m\t=> %s\n", name.c_str(), "[ \033[0;32;32mOK\033[0m  ]");
        }
        else
        {

            printf("\033[1;33mTest: %s\033[0m\t=> %s\n", name.c_str(), "[ \033[1;31;40mFail\033[0m ]");
        }
        unit->performance();
    }

    printf("\033[1;36mTest: passed %d/%d\033[0m\n", (sum - fail), sum);
    return 0;

    return 0;
}
