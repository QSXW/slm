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

void Matrix4Transpose()
{
    slm::Timer t;
    static float a[4 * 4] = { 0 };

    RandomBuffer<float, 4, 4>(a);

    DirectX::XMMATRIX *d = (DirectX::XMMATRIX *)&a;
    glm::mat4         *g = (glm::mat4 *)&a;
    slm::Matrix4      *s = (slm::Matrix4 *)&a;

    Test::Regress<100>([&]()
    {
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
        }
    });

    if (Benchmarks::Regressed)
    {
        static constexpr size_t times = 1000000;
        FILE *fp = fopen("test.f", "wb+");
        gbuffer = (float *)malloc(sizeof(float) * times);
        {
            Succeed("DirectX::XMMatrixTranspose()");
            slm::Timer t;
            RegressV<times>(DirectX::XMMatrixTranspose, *d);
        }
        fwrite(gbuffer, sizeof(float) * times, 1, fp);
        {
            Succeed("glm::transpose()");
            slm::Timer t;
            RegressX(times, glm::transpose(*g));
        }
        // fwrite(gbuffer, sizeof(float) * times, 1, fp);
        // {
        //     Succeed("nlm::Dot()");
        //     slm::Timer t;
        //     RegressV<times>(nlm::dot, *v1, *v2);
        // }
        fwrite(gbuffer, sizeof(float) * times, 1, fp);
        {
            Succeed("slm::transpose();");
            slm::Timer t;
            RegressV<times>(slm::transpose, *s);
        }
        remove("test.f");
        fclose(fp);
        free(gbuffer);
    }
}

} // namespace Check
} // namespace Test
