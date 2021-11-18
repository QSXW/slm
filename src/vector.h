#pragma once

#include <iostream>
#include "sl.h"
#include "slintrinsic.h"

namespace slm
{

using namespace sl;

namespace avx
{

extern "C" void  slm_vec_add(void *v1, void *v2, void *v3);
extern "C" void  slm_vec_sub(void *v1, void *v2, void *v3);
extern "C" void  slm_vec_mul(void *v1, const float factor, void *v3);
extern "C" float slm_vec_dot(void *v1, void *v2);

struct Vector;
inline float dot(const Vector& u, const Vector& v)
{
    m128 t, r;

    t = load<m128>(&u);
    r = load<m128>(&v);

    t = t * r;
    r = _mm_movehl_ps(t, t);
    t = t + r;
    r = _mm_movehdup_ps(t);
    t = t + r;

    return *((float*)&t);
}

SL_ALIGNED_STRUCT(16)  Vector
{
public:
    Vector()
        : x(0), y(0), z(0), w(0)
    {

    }

    __fastcall Vector(float n)
    {
        auto t = _mm_set1_ps(n);
        _mm_store_ps(rcast<float *>(this), t);
    }

    Vector(float x, float y, float z)
        : x(x), y(y), z(z), w(0)
    {

    }

    Vector(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w)
    {

    }

    void Add(const Vector &v)
    {
        slm_vec_add((void *)this, (void *)&v, (void *)this);
    }

    void Sub(const Vector &v)
    {
        slm_vec_sub((void *)this, (void *)&v, (void *)this);
    }

    void Mul(const float factor)
    {
        slm_vec_mul((void *)this, factor, (void *)this);
    }

    float Dot(const Vector &v) const
    {
        return dot(*this, v);
    }

    Vector operator+=(const Vector &v)
    {
        this->Add(v);
        return *this;
    }

    Vector operator-=(const Vector &v)
    {
        this->Sub(v);
        return *this;
    }

    Vector operator*=(const float factor)
    {
        this->Mul(factor);
        return *this;
    }

    Vector operator+(const Vector &v)
    {
        Vector r;
        slm_vec_add((void *)this, (void *)&v, (void *)&r);
        return r;
    }

    Vector operator-(const Vector &v)
    {
        Vector r;
        slm_vec_sub((void *)this, (void *)&v, (void *)&r);
        return r;
    }

    Vector operator*(const float factor)
    {
        Vector r;
        slm_vec_mul((void *)this, factor, (void *)&r);
        return r;
    }

    friend std::ostream & operator << (std::ostream &o, const Vector &m);

public:
    float x;
    float y;
    float z;
    float w;
};

inline Vector operator*(const float factor, Vector &v)
{
    return v * factor;
}

inline float length(const Vector& v)
{
    return sqrt(dot(v, v));
}

inline std::ostream & operator << (std::ostream &o, const Vector &v)
{
    printf("%6.2f\t%6.2f\t%6.2f\t%6.2f\n", v.x, v.y, v.z, v.w);
    return o;
}

}

inline namespace version
{
    using namespace avx;
}

}