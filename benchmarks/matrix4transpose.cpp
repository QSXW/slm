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
    slm::Timer t;
    static float a[4 * 4] = { 0 };

    RandomBuffer<float, 4, 4>(a);

    DirectX::XMMATRIX *d = (DirectX::XMMATRIX *)&a;
    glm::mat4         *g = (glm::mat4 *)&a;
    slm::Matrix4      *s = (slm::Matrix4 *)&a;

    Test::RandomBuffer<float, 4, 4>(a);

    std::cout << "Before Transpose: " << std::endl;
    std::cout << *s << std::endl;
    std::cout << std::endl;

    auto m1 = DirectX::XMMatrixTranspose(*d);
    auto m2 = glm::transpose(*g);
    auto m3 = s->Transpose();

    std::cout << "drx::transpose => " << std::endl;
    std::cout << *((slm::Matrix4 *)&m1) << std::endl;

    std::cout << "glm::transpose => " << std::endl;
    std::cout << *((slm::Matrix4 *)&m2) << std::endl;

    std::cout << "slm::transpose => " << std::endl;
    std::cout <<  m3 << std::endl;

    if (!CompareFloatingPointSequence<DirectX::XMMATRIX, slm::Matrix4, 4, 4>(m1, m3, 0.01f) &&
    !CompareFloatingPointSequence<glm::mat4, slm::Matrix4, 4, 4>(m2, m3, 0.01f))
    {
        Fail("Failed to pass the %s\n", __func__);
        return false;
    }

    if (Benchmarks::Regressed)
    {
        static constexpr size_t times = 1000000;
        gbuffer = (float *)malloc(sizeof(float) * times);
        {
            Succeed("DirectX::XMMatrixTranspose()");
            slm::Timer t;
            RegressV<times>(DirectX::XMMatrixTranspose, *d);
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
            RegressV<times>(slm::Matrix4::transpose, *s);
        }
        Submit<float, times>(gbuffer);
        free(gbuffer);
    }

    return true;
}

} // namespace Check
} // namespace Test
