#include <iostream>

#include "benchmarks.h"
#include "timer.h"

#include "DirectXMath.h"
#include "slm.h"
#include "nlm.h"
#include "glm/glm.hpp"

namespace Test
{

class Vector4 : public Unit
{
public:
    virtual const std::string name() const override
    {
        return "Vector4";
    }

    virtual bool comformance() const override
    {
        static float a[4] = { 0 };
        static float b[4] = { 0 };
        static float c[4] = { 0 };

        DirectX::XMVECTOR *d1 = (DirectX::XMVECTOR *)(&a);
        DirectX::XMVECTOR *d2 = (DirectX::XMVECTOR *)(&b);

        glm::vec4 *g1 = (glm::vec4 *)(&a);
        glm::vec4 *g2 = (glm::vec4 *)(&b);

        slm::Vector *v1 = (slm::Vector *)(&a);
        slm::Vector *v2 = (slm::Vector *)(&b);

        randomize<4, 1, float>(a);
        randomize<4, 1, float>(b);
        BENCH_OUT(data, a, *v1);
        BENCH_OUT(data, b, *v2);

        auto ans1 = DirectX::XMVector4Dot(*d1, *d2);
        auto ans2 = glm::dot(*g1, *g2);
        auto ans3 = nlm::dot(*v1, *v2);
        auto ans4 = slm::dot(*v1, *v2);

        BENCH_OUT(drx, dot, ((float *)&ans1)[0]);
        BENCH_OUT(glm, dot, ans2);
        BENCH_OUT(nlm, dot, ans3);
        BENCH_OUT(slm, dot, ans4);

        if (!(compare_float(((float *)&ans1)[0], ans4) && compare_float(ans2, ans4)))
        {
            return false;
        }

        static constexpr size_t times = 10000000;
        auto gbuffer = (float *)malloc(sizeof(float) * times);
        {
            Succeed("DirectX::XMVector4Dot()");
            slm::Timer t;
            RegressX(times, DirectX::XMVector4Dot(*d1, *d2));
        }
        Submit<float, times>(gbuffer);
        {
            Succeed("glm::dot()");
            slm::Timer t;
            RegressX(times, glm::dot(*g1, *g2));
        }
        Submit<float, times>(gbuffer);
        {
            Succeed("nlm::Dot()");
            slm::Timer t;
            RegressX(times, nlm::dot(*v1, *v2));
        }
        Submit<float, times>(gbuffer);
        {
            Succeed("slm::Dot();");
            slm::Timer t;
            RegressX(times, slm::dot(*v1, *v2));
        }
        Submit<float, times>(gbuffer);
        free(gbuffer);

        return true;
    }
};

}
