#include "benchmarks.h"

#include "DirectXMath.h"
#include "slm.h"
#include "nlm.h"
#include "glm/glm.hpp"
#include "timer.h"

namespace Test
{

class Matrix4 : public Unit
{
public:
    virtual bool comformance() const override
    {
        float SL_ALIGNED(64) a[512] = { 0 };
        RandomBuffer<4, 4, float>(a);

        const auto d = sl::rcast<DirectX::XMMATRIX *>(&a);
        const auto g = sl::rcast<glm::mat4 *>(&a);
        const auto s = sl::rcast<slm::Matrix4 *>(&a);

        auto m1 = DirectX::XMMatrixTranspose(*d);
        auto m2 = glm::transpose(*g);
        auto m3 = s->Transpose();

        BENCH_OUTL(data, matrix4, *s);
        BENCH_OUTL(drx, Matrix4Transpose, *((slm::Matrix4 *)&m1))
        BENCH_OUTL(glm, Matrix4Transpose, *((slm::Matrix4 *)&m2))
        BENCH_OUTL(slm, Matrix4Transpose, *((slm::Matrix4 *)&m3))

        if (!Equals<4, 4>(m1, m3) && !Equals<4, 4>(m2, m3))
        {
            return false;
        }

        static constexpr size_t times = 10000000;
        auto gbuffer = (float *)malloc(sizeof(float) * times);
        {
            Succeed("DirectX::XMMatrixTranspose()");
            slm::Timer t;
            RegressX(times, DirectX::XMMatrixTranspose(*d));
        }
        Submit<float, times>(gbuffer);
        {
            Succeed("glm::transpose()");
            slm::Timer t;
            RegressX(times, glm::transpose(*g));
        }
        Submit<float, times>(gbuffer);
        {
            Succeed("slm::transpose();");
            slm::Timer t;
            RegressX(times, slm::Matrix4::transpose(*s));
        }
        Submit<float, times>(gbuffer);
        free(gbuffer);

        return true;
    }

    virtual void performance() const override
    {

    }

    virtual const std::string name() const override
    {
        return "Matrix4";
    }
};

}
