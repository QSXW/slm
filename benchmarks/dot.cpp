#include <iostream>

#include "benchmarks.h"
#include "timer.h"

#include "DirectXMath.h"
#include "slm.h"
#include "nlm.h"
#include "glm/glm.hpp"

namespace Test
{
namespace Check
{

void dot()
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

    Regress<100>([&]() {
        RandomBuffer<float, 4, 1>(a);
        RandomBuffer<float, 4, 1>(b);

        std::cout << "a: \t" << *v1;
        std::cout << "b: \t" << *v2;

        auto ans1 = DirectX::XMVector4Dot(*d1, *d2);
        auto ans2 = glm::dot(*g1, *g2);
        auto ans3 = nlm::dot(*v1, *v2);
        auto ans4 = slm::dot(*v1, *v2);

        std::cout << "drx::dot => \t" << ((float *)&ans1)[0] << std::endl;
        std::cout << "glm::dot => \t" << ans2 << std::endl;
        std::cout << "nlm::dot => \t" << ans3 << std::endl;
        std::cout << "slm::dot => \t" << ans4 << std::endl;
        std::cout << std::endl;

        if (!(Test::CompareFloatingPoint(((float *)&ans1)[0], ans4) && Test::CompareFloatingPoint(ans2, ans4)))
        {
            Fail("Failed to pass test\n");
        }
    });

    if (Benchmarks::Regressed)
    {
        static constexpr size_t times = 10000000;
        FILE *fp = fopen("test.f", "wb+");
        gbuffer = (float *)malloc(sizeof(float) * times);
        {
            Succeed("DirectX::XMVector4Dot()");
            slm::Timer t;
            RegressV<times>(DirectX::XMVector4Dot, *d1, *d2);
        }
        fwrite(gbuffer, sizeof(float) * times, 1, fp);
        {
            Succeed("glm::dot()");
            slm::Timer t;
            RegressX(times, glm::dot(*g1, *g2));
        }
        fwrite(gbuffer, sizeof(float) * times, 1, fp);
        {
            Succeed("nlm::Dot()");
            slm::Timer t;
            RegressV<times>(nlm::dot, *v1, *v2);
        }
        fwrite(gbuffer, sizeof(float) * times, 1, fp);
        {
            Succeed("slm::Dot();");
            slm::Timer t;
            RegressV<times>(slm::dot, *v1, *v2);
        }
        remove("test.f");
        fclose(fp);
        free(gbuffer);
    }
}

} // namespace Check
} // namespace Test
