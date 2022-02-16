#pragma once

#include <cstdint>
#include <cstdlib>
#include <iostream>

extern "C" int slm_submit_result(void *ptr, size_t size);

namespace Test
{

template <class T, size_t size>
int Submit(T *ptr)
{
    return slm_submit_result(ptr, sizeof(T) * size);
}

static float *gbuffer = nullptr;

bool CompareFloatingPoint(float a, float b, float epsilon = 0.01f);

struct Benchmarks
{
    const char *name;
    void (*func)(void);

    static bool Regressed;
};

template <class... Args>
inline constexpr void Fail(const char *s, Args&&... args)
{
    fprintf(stderr, s, std::forward<Args>(args)...);
}

template <class... Args>
inline constexpr void Succeed(const char *s, Args&&... args)
{
    printf(s, std::forward<Args>(args)...);
}

template <size_t count, class T, class... Args>
inline constexpr void Regress(T func, Args&&... args)
{
    for (int i = 0; i < count; i++)
    {
        func(std::forward<Args>(args)...);
    }

}

template <size_t count, class T, class... Args>
inline constexpr void RegressV(T func, Args&&... args)
{
    float *ptr = gbuffer;
    for (volatile int i = 0; i < count; i++)
    {
        auto ans = func(std::forward<Args>(args)...);
        *ptr++ = *((float *)&ans);
    }
}

#define RegressX(count, command) { \
    float *ptr = gbuffer; \
    for (volatile int i = 0; i < count; i++) \
    { \
        auto ans = command; \
        *ptr++ = *((float *)&ans); \
    } \
}

template <size_t rows, size_t cols, class T>
inline constexpr void RandomBuffer(T *buffer)
{
    auto size = rows * cols * sizeof(T);
    for (volatile int i = 0; i < size; i++)
    {
        buffer[i] = (T)(rand() & 0xFF);
    }
}

template <size_t rows, size_t cols, class T, class V>
inline constexpr bool CompareFloatingPointSequence(const T u, const V v, float epsilon)
{
    float *a = (float *)&u;
    float *b = (float *)&v;
    for (size_t i = 0, length = rows * cols; i < length; i++)
    {
        if (!CompareFloatingPoint(a[i], b[i], epsilon))
        {
            return false;
        }
    }
    return true;
}

#define BENCH_OUT(S, N, V) \
    std::cout << #S"::"#N": " <<  V << std::endl;

#define BENCH_OUTL(S, N, V) \
    std::cout << #S"::"#N": \n" <<  V << std::endl;

} // namespace Test
