#pragma once

#include <cstdint>
#include <cstdlib>
#include <iostream>

extern "C" int slm_submit_result(void *ptr, size_t size);

namespace Test
{

class Unit
{
public:
    virtual const std::string name() const = 0;

    virtual bool comformance() const = 0;

    virtual void performance() const { }
};

template <class T, size_t size>
int Submit(T *ptr)
{
    return slm_submit_result(ptr, sizeof(T) * size);
}

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

#define Succeed printf

#define RegressX(count, command) { \
    float *ptr = gbuffer; \
    for (volatile int i = 0; i < count; i++) \
    { \
        auto ans = command; \
        *ptr++ = *((float *)&ans); \
    } \
}

template <size_t rows, size_t cols, class T>
inline constexpr void randomize(T *buffer)
{
    auto size = rows * cols * sizeof(T);
    for (volatile int i = 0; i < size; i++)
    {
        buffer[i] = (T)(rand() & 0xFF);
    }
}

bool compare_float(float a, float b, float epsilon = 0.01f)
{
    float diff = fabsf(a - b);

    if (diff < epsilon)
    {
        return true;
    }
    Fail( "Test failed comparing %g with %g (abs diff=%g with epsilon=%g)\n", a, b, diff, epsilon);

    return false;
}

template <size_t rows, size_t cols, class T, class V>
inline constexpr bool equals(const T u, const V v, float epsilon = 0.01f)
{
    float *a = nullptr;
    float *b = nullptr;
    if constexpr (std::is_pointer<T>())
    {
        a = (float *)u;
        b = (float *)v;
    }
    else
    {
        a = (float *)&u;
        b = (float *)&v;
    }
    for (size_t i = 0, length = rows * cols; i < length; i++)
    {
        if (!compare_float(a[i], b[i], epsilon))
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
