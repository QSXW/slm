#pragma once

#include <iostream>
#include "slcore.h"

namespace slm
{
    template <class T, class O>
    inline constexpr T Cast(O &o)
    {
        return *(T *)(&o);
    }
}

extern "C" void  slm_vec_add(void *v1, void *v2, void *v3);
extern "C" void  slm_vec_sub(void *v1, void *v2, void *v3);
extern "C" void  slm_vec_mul(void *v1, const float factor, void *v3);
extern "C" float slm_vec_dot(void *v1, void *v2);

extern "C" void slm_mat4_mul(void *m, void *v1, void *v2);
extern "C" void slm_mat4_transpose(void*m1, void *m2);

namespace slm
{
    namespace avx
    {
        struct Vector;
        inline float dot(const Vector& u, const Vector& v)
        {
            __m128 t, r;

            t = _mm_mul_ps(*(__m128*) & u, *(__m128*) & v);
            r = _mm_movehl_ps(t, t);
            t = _mm_add_ps(t, r);
            r = _mm_movehdup_ps(t);
            t = _mm_add_ps(t, r);

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
                _mm_store_ps(RCAST<float *>(this), t);
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

        SL_ALIGNED_STRUCT(64) Matrix4
        {
        public:
            static constexpr int Size = 4*4;
            static constexpr int RowNumber = 4;

        public:
            __fastcall Matrix4()
            {
                auto t = _mm512_set1_ps(0.0f);
                _mm512_store_ps(RCAST<float *>(this), t);
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
            auto r = _mm512_permutexvar_ps(t, Cast<__m512>(m));
            return Cast<Matrix4>(r);
        }
    }
    
    inline namespace version
    {
        using namespace avx;
    }
    
}


