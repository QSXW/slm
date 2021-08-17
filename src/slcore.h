#pragma once

#if defined( _MSC_VER )
#   define SL_ALIGNED(x) __declspec(align(x))
#	define SL_ALIGNED_STRUCT(x) struct __declspec(align(x))
#	define SL_ALIGNED_CLASS(x) class __declspec(align(x)))
#   define SL_ASSEMBLY __asm
#include <immintrin.h>

#pragma warning(disable: 4996)

#elif defined( __GNUC__)
#   define SL_ALIGNED(x) __attribute__((aligned(x)))
#	define SL_ALIGNED_STRUCT(x) struct __attribute__((aligned(x)))
#	define SL_ALIGNED_CLASS(x) class __attribute__((aligned(x)))
#   define SL_ASSEMBLY __asm__
#include <immintrin.h>
#endif

namespace slm
{
	template <class T, class O>
	inline constexpr T RCAST(O p)
	{
		return reinterpret_cast<T>(p);
	}
}