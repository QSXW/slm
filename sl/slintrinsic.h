#pragma once

#ifndef __SLINTRINSIC_H__
#define __SLINTRINSIC_H__

#include <immintrin.h>
#include <iostream>

#include "slapi.h"
#include "slcast.h"

using namespace sl;

#define SL_DONT_USE_DIV [[deprecated("avoid to use, try convert to a multiply before broadcast")]]

#define CONSTRUCTOR_PRIMITIVE() R(Primitive v) : v{ v } {} operator Primitive() const { return v; };
#define CONSTURCTOR_SET1(P, S, T) R(T n) : v{ _m##P##_set1_##S(n) } {}
#define CONSTURCTOR_LOAD(P, S, T) R(const void *data) : v{ _m##P##_load_##S((T *)data) } {}

#define DEFINE_OPERATOR(P, N, O, T) R &operator N (R &b) { v = _m##P##_##O##_##T(v, b.v); return *this; }
#define OPEARTOR_MUL(P, T) DEFINE_OPERATOR(P, *, mul, T)
#define OPEARTOR_ADD(P, T) DEFINE_OPERATOR(P, +, add, T)

struct int32x4
{
#define R int32x4
    using Primitive = __m128i;
    CONSTURCTOR_SET1(m, epi8,   int8_t)
    CONSTURCTOR_SET1(m, epi16,  int16_t)
    CONSTURCTOR_SET1(m, epi32,  int32_t)
    CONSTURCTOR_SET1(m, epi64x, int64_t)
    CONSTURCTOR_LOAD(m, si128,  Primitive)

#undef R
private:
    Primitive v;
};

struct floatx4
{
#define R floatx4
public:
    using Primitive = __m128;

public:
    floatx4() = default;

    CONSTRUCTOR_PRIMITIVE()
    CONSTURCTOR_SET1(m, ps, float)
    CONSTURCTOR_LOAD(m, ps, float)

    OPEARTOR_MUL(m, ps)

    floatx4 &operator+(floatx4 &other)
    {
        v = _mm_add_ps(v, other.v);
        return *this;
    }

    operator float() const
    {
        return *((float*)&v);
    }

    floatx4 movehl(floatx4 &other)
    {
        return _mm_movehl_ps(v, other.v);
    }

    floatx4 movehdup() const
    {
        return _mm_movehdup_ps(v);
    }

#undef R
private:
    Primitive v;
};

struct int32x8
{
#define R int32x8
    using Primitive = __m256i;
    CONSTURCTOR_SET1(m256, epi8,   int8_t)
    CONSTURCTOR_SET1(m256, epi16,  int16_t)
    CONSTURCTOR_SET1(m256, epi32,  int32_t)
    CONSTURCTOR_SET1(m256, epi64x, int64_t)
    CONSTURCTOR_LOAD(m256, si256, Primitive)

#undef R
private:
    Primitive v;
};

struct int32x16
{
#define R int32x16
public:
    using Primitive = __m512i;

public:
    CONSTRUCTOR_PRIMITIVE();
    CONSTURCTOR_SET1(m512, epi8,   int8_t)
    CONSTURCTOR_SET1(m512, epi16,  int16_t)
    CONSTURCTOR_SET1(m512, epi32,  int32_t)
    CONSTURCTOR_SET1(m512, epi64, int64_t)
    CONSTURCTOR_LOAD(m512, si512, Primitive)

#undef R
private:
    Primitive v;
};

struct floatx16
{
#define R floatx16
public:
    using Primitive = __m512;

public:
    floatx16() = default;

    CONSTRUCTOR_PRIMITIVE()
    CONSTURCTOR_SET1(m512, ps, float)
    CONSTURCTOR_LOAD(m512, ps, float)

    Primitive permutexvar_ps(const int32x16 &vindex)
    {
       return _mm512_permutexvar_ps(vindex, v);
    }

#undef R
private:
    Primitive v;
};

#endif /* __SLINTRINSIC_H__ */
