#include <iostream>

#include "benchmarks.h"
#include "timer.h"
#include "slm.h"

namespace Test
{

class Transform : public Unit
{
public:
    virtual const std::string name() const override
    {
        return "Transform";
    }

    virtual bool comformance() const override
    {
        float src[4] = { 11.0, 14.0f, 3.0f, 24.0f };
        float dst[4] = { 0 };

        float dct_res[4]  = { 26,  -5.51608072, 9, -10.70387096 };
        float idct_res[4] = { 22.64029396, -7.89038288, 15.89038288, -8.64029396 };

        slm::dct1d(dst, src, 4);
        if (!equals<4, 1>(dst, dct_res))
        {
            return false;
        }

        slm::idct1d(dst, src, 4);
        if (!equals<4, 1>(dst, idct_res))
        {
            return false;
        }

        return true;
    }
};

}
