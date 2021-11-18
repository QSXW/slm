#pragma once

#include <cstdint>
#include <cstdlib>
#include <iostream>

namespace Test
{

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
    __debugbreak();
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
    for (int i = 0; i < count; i++)
    {
        auto ans = func(std::forward<Args>(args)...);
        *ptr++ = *((float *)&ans);
    }
}

#define RegressX(count, command) { \
    float *ptr = gbuffer; \
    for (int i = 0; i < count; i++) \
    { \
        auto ans = command; \
        *ptr++ = *((float *)&ans); \
    } \
}

template <class T, size_t rows, size_t cols>
inline constexpr void RandomBuffer(T *buffer)
{
    auto size = rows * cols * sizeof(T);
    for (int i = 0; i < size; i++)
    {
        buffer[i] = (T)(rand() & 0xFF);
    }
}

template <class T, class V, size_t rows, size_t cols>
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

} // namespace Test
