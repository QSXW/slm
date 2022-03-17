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

static const double A[] = {
	NAN,
	0.707106781186547524400844,
	0.541196100146196984399723,
	0.707106781186547524400844,
	1.306562964876376527856643,
	0.382683432365089771728460,
};

static const double S[] = {
    0.353553390593273762200422,
    0.254897789552079584470970,
    0.270598050073098492199862,
    0.300672443467522640271861,
    0.353553390593273762200422,
    0.449988111568207852319255,
    0.653281482438188263928322,
    1.281457723870753089398043,
};

#define IDCT8(C, stride) \
	v15 = C[stride * 0] / S[0]; \
    v26 = C[stride * 1] / S[1]; \
    v21 = C[stride * 2] / S[2]; \
    v28 = C[stride * 3] / S[3]; \
	v16 = C[stride * 4] / S[4]; \
    v25 = C[stride * 5] / S[5]; \
    v22 = C[stride * 6] / S[6]; \
    v27 = C[stride * 7] / S[7]; \
	v19 = (v25 - v28) / 2; \
    v20 = (v26 - v27) / 2; \
    v23 = (v26 + v27) / 2; \
    v24 = (v25 + v28) / 2; \
	v7  = (v23 + v24) / 2; \
    v11 = (v21 + v22) / 2; \
    v13 = (v23 - v24) / 2; \
    v17 = (v21 - v22) / 2; \
	v8  = (v15 + v16) / 2; \
    v9  = (v15 - v16) / 2; \
	v18 = (v19 - v20) * A[5]; \
	v12 = (v19 * A[4] - v18) / (A[2] * A[5] - A[2] * A[4] - A[4] * A[5]); \
	v14 = (v18 - v20 * A[2]) / (A[2] * A[5] - A[2] * A[4] - A[4] * A[5]); \
	v6 = v14 - v7; \
	v5 = v13 / A[3] - v6; \
	v4 = -v5 - v12; \
	v10 = v17 / A[1] - v11; \
	v0 = (v8 + v11) / 2; \
	v1 = (v9 + v10) / 2; \
	v2 = (v9 - v10) / 2; \
	v3 = (v8 - v11) / 2; \
	C[stride * 0] = (v0 + v7) / 2; \
	C[stride * 1] = (v1 + v6) / 2; \
	C[stride * 2] = (v2 + v5) / 2; \
	C[stride * 3] = (v3 + v4) / 2; \
	C[stride * 4] = (v3 - v4) / 2; \
	C[stride * 5] = (v2 - v5) / 2; \
	C[stride * 6] = (v1 - v6) / 2; \
	C[stride * 7] = (v0 - v7) / 2;

#define IDCT8_H(C, O) IDCT8((C + O), 1)
#define IDCT8_V(C, O) IDCT8((C + O), 8)

template <class T>
void idct8_2d(T *block)
{
    double  v0,  v1,  v2,  v3,  v4,  v5,  v6,  v7;
    double  v8,  v9, v10, v11, v12, v13, v14, v15;
    double v16, v17, v18, v19, v20, v21, v22, v23;
    double v24, v25, v26, v27, v28;

    IDCT8_H(block, 0 );
    IDCT8_H(block, 8 );
    IDCT8_H(block, 16);
    IDCT8_H(block, 24);
    IDCT8_H(block, 32);
    IDCT8_H(block, 48);
    IDCT8_H(block, 56);

    IDCT8_V(block, 0);
    IDCT8_V(block, 1);
    IDCT8_V(block, 2);
    IDCT8_V(block, 3);
    IDCT8_V(block, 4);
    IDCT8_V(block, 5);
    IDCT8_V(block, 6);
    IDCT8_V(block, 7);
}

}
