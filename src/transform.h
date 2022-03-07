#pragma once

#include "utils.h"
#include "trigonometric.h"

namespace slm
{

static inline float wave(size_t x, size_t frequency, size_t n)
{
    return cos((x + 0.5) * frequency * PI / n);
}

template <class T>
void dct1d(T *F, T *const f, size_t n)
{
    assert(n != 0);
    T sum = 0;

    for (size_t x = 0; x < n; x++)
    {
        sum += f[x];
    }
    F[0] = sum * 1 / sqrt(T(n));

    for (size_t i = 1; i < n; i++)
    {
        sum = 0;
        for (size_t x = 0; x < n; x++)
        {
            sum += f[x] * wave(x, i, n);
        }
        F[i] = sum * sqrt(2.0f / n);
    }
}

template <class T>
void idct1d(T *f, T const *F, size_t n)
{
    assert(n != 0);

    for (size_t x = 0; x < n; x++)
    {
        T sum = 0;
        for (size_t i = 1; i < n; i++)
        {
            sum += F[i] * wave(x, i, n);
        }
        f[x] = sum * sqrt(2.0f / n) + F[0] * 1 / sqrt(T(n));
    }
}

}
