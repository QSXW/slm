#pragma once

#include <iostream>

#include "sl.h"
#include "slintrinsic.h"
#include "vector.h"

extern "C" void slm_mat4_mul(void *m, void *v1, void *v2);
extern "C" void slm_mat4_transpose(void*m1, void *m2);

namespace slm
{

using namespace sl;

SL_ALIGNED_STRUCT(64) Matrix4
{
public:
    static constexpr int Size = 4*4;
    static constexpr int RowNumber = 4;

public:
    __fastcall Matrix4()
    {
        auto t = _mm512_set1_ps(0.0f);
        _mm512_store_ps(rcast<float *>(this), t);
    }

    Vector& operator[](size_t index)
    {
        return rows.v4[index];
    }

    Vector operator[](size_t index) const
    {
        return rows.v4[index];
    }

    Vector Mul(const Vector &v)
    {
        Vector r;
        slm_mat4_mul((void *)this, (void *)&v, (void *)&r);
        return r;
    }

    Matrix4 Transpose()
    {
        Matrix4 m;
        slm_mat4_transpose((void *)this, (void *)&m);
        return m;
    }

    friend std::ostream & operator << (std::ostream &o, const Matrix4 &m);

private:
    union U
    {
        float  f16[16];
        Vector v4[4];

        U() {};
    } rows;
};

inline std::ostream & operator << (std::ostream &o, const Matrix4 &m)
{
    for (size_t i = 0; i < Matrix4::RowNumber; i++)
    {
        o << m[i];
    }
    return o;
}

inline Matrix4 transpose(const Matrix4 &m)
{
    static const int SL_ALIGNED(64) indices[] = {
        0, 4,  8, 12,
        1, 5,  9, 13,
        2, 6, 10, 14,
        3, 7, 11, 15
    };

    auto t = _mm512_load_epi32(indices);
    auto r = _mm512_permutexvar_ps(t, bcast<__m512>(m));
    return bcast<Matrix4>(r);
}

}
