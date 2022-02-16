#include "benchmarks.h"

#include "DirectXMath.h"
#include "slm.h"
#include "nlm.h"
#include "glm/glm.hpp"
#include "timer.h"

namespace Test
{
namespace Check
{

bool Matrix4Transpose()
{
    static float a[4 * 4] = { 0 };

    RandomBuffer<4, 4, float>(a);

    DirectX::XMMATRIX *d = (DirectX::XMMATRIX *)&a;
    glm::mat4         *g = (glm::mat4 *)&a;
    slm::Matrix4 SL_ALIGNED(64) *s = (slm::Matrix4 *)&a;

    Test::RandomBuffer<4, 4, float>(a);

    auto m1 = DirectX::XMMatrixTranspose(*d);
    auto m2 = glm::transpose(*g);
    auto m3 = s->Transpose();

    BENCH_OUTL(data, matrix4, *s);
    BENCH_OUTL(drx, Matrix4Transpose, *((slm::Matrix4 *)&m1))
    BENCH_OUTL(glm, Matrix4Transpose, *((slm::Matrix4 *)&m2))
    BENCH_OUTL(slm, Matrix4Transpose, *((slm::Matrix4 *)&m3))

    if (!CompareFloatingPointSequence<4, 4>(m1, m3, 0.01f) && !CompareFloatingPointSequence<4, 4>(m2, m3, 0.01f))
    {
        return false;
    }

    if (Benchmarks::Regressed)
    {
        static constexpr size_t times = 10000000;
        gbuffer = (float *)malloc(sizeof(float) * times);
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
    }

    return true;
}

} // namespace Check
} // namespace Test
