#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <new>
#include <string>

#ifdef _MSC_VER
#   define SL_ALIGNED(x) __declspec(align(x))
#	define SL_ALIGNED_STRUCT(x) struct __declspec(align(x))
#	define SL_ALIGNED_CLASS(x) class __declspec(align(x)))
#   define SL_ASSEMBLY __asm

#   define sl_disable_optimizations optimize("", off)
#   define sl_enable_optimizations  optimize("", on)

#pragma warning(disable: 4996)

#elif defined(__GNUC__)
#   define SL_ALIGNED(x) __attribute__((aligned(x)))
#	define SL_ALIGNED_STRUCT(x) struct __attribute__((aligned(x)))
#	define SL_ALIGNED_CLASS(x) class __attribute__((aligned(x)))
#   define SL_ASSEMBLY __asm__
#endif

namespace sl
{

template <class T, size_t align>
inline T *aligned_malloc(size_t size)
{
    T *ptr = nullptr;
#ifdef _MSC_VER
    ptr = ::_aligned_malloc(size * sizeof(T), align);
#elif defined(__GNUC__) && defined(__linux__)
    ptr = aligned_alloc(size * sizeof(T), align);
#endif
    return ptr ? static_cast<T *>(ptr) : throw std::bad_alloc{};
}

inline void aligned_free(void* ptr)
{
#ifdef __GNUC__
    std::free(ptr);
#else
    ::_aligned_free(ptr);
#endif
}

#define SLASSERT(...) assert(__VA_ARGS__)
#define SLBIND(x) std::bind(&x, this, std::placeholders::_1)
#define SLDEBUG defined( DEBUG) || defined( _DEBUG )

#if defined( WIN32 ) || defined( _WIN32 )
#   define WINDOWS
#   define SLSURFACE "VK_KHR_win32_surface"
#elif defined( __ANDROID__ )
#   define ANDROID
elif defined(__linux__)
#   define LINUX
#elif defined( __APPLE__ ) || defined( __MACH__ )
#   define APPLE
#endif

/*
 * @brief: Helper Macro
 */
#define U8(str) u8##str
#define BIT(x) (1 << (x))
#define CAT1(p, s) #p##s
#define SLALIGN(x, a) (((x) + (a) - 1) & ~((a) - 1))
#define SLROTATE(a, b) (a) = ((a) + 1) % (b)
#define SL_ARRAY_LENGTH(a) sizeof(a) / sizeof((a)[0])

/*
 * @brief: Type Definitions
 */
using INT8   = char;
using UINT8  = unsigned char;
using INT16  = short;
using UINT16 = unsigned short;
using INT32  = int;
using UINT32 = unsigned int;
using INT64  = int64_t;
using UINT64 = uint64_t;

using Anonymous = void*;

/*
 * @brief: Helper Template
 */
template <class T, class U>
inline constexpr bool IsReferenceOf()
{
    return std::is_same<T&, U>();
}

template <class T, class U>
inline constexpr bool IsPrimitiveOf()
{
    return std::is_same<T, U>() || IsReferenceOf<T, U>();
}

template <class T, class U>
inline constexpr bool IsPointerOf()
{
    return std::is_same<T*, U>();
}

template <class T1, class T2, class T3>
inline constexpr bool IsPrimitiveOf()
{
    if constexpr (std::is_same_v<T1, T2> || std::is_same_v<T1, T3>)
    {
        return true;
    }
    return false;
}

template <class T1, class T2, class T3, class T4>
inline constexpr bool IsPrimitiveOf()
{
    if constexpr (std::is_same_v<T1, T2> ||
        std::is_same_v<T1, T3> ||
        std::is_same_v<T1, T4>)
    {
        return true;
    }
    return false;
}

template <class T>
inline constexpr void *CleanUpObject(T *ptr, int value = 0, size_t size = sizeof(T))
{
    return memset(ptr, value, size);
}

template <class T>
inline constexpr void CopyProps(T *dst, const T *src)
{
    memcpy(dst, src, sizeof(T));
}

static inline bool Equals(const char *str1, const char *str2)
{
    return (strcmp(str1, str2) == 0);
}

class Exception : public std::exception
{
public: Exception(const char *what) noexcept :
#ifdef _MSC_VER
std::exception(what, 1) { }
#elif __GNUC__
message(what) { }

    virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override
    {
        return message.c_str();
    }

    std::string message;
#endif
};

class RuntimeException : public Exception
{
public:
    RuntimeException(const char *what) noexcept :
        Exception(what)
    {

    }

    RuntimeException(const std::string &what) noexcept :
        RuntimeException(what.c_str())
    {

    }
};

class StaticException : public Exception
{
public:
    StaticException(const char *what) noexcept :
        Exception(what)
    {

    }

    StaticException(const std::string &what) noexcept :
        StaticException(what.c_str())
    {

    }
};

/*
 * @brief Static Exception means the case should not occur anyway when running.
 *  Using RuntimeException to check runtime error instead.
 */
#ifdef SLDEBUG
#define THROWIF(expr, what) \
    if (!!(expr)) \
    { \
        throw StaticException(what); \
    }
#else
#define THROWIF
#endif

#define ThrowIf(expr, what) \
    if (!!(expr)) \
    { \
        throw RuntimeException(what); \
    }

#define DERROR(name, str) static constexpr const char name[] = str;
namespace SError
{
    DERROR(OutOfBound,           "Index was out of bound"                  )
    DERROR(OutOfMemory,          "No more memory on the runtime"           )
    DERROR(InvalidSingleton,     "Cannot construct more than one singleton")
    DERROR(SelfAssignment,       "Self-assignment is not permitted"        )
    DERROR(NullPointerReference, "Null pointer reference"                  )
    DERROR(UnableToOpenFile,     "Unable to open the file specified"       )
}

#define SL_DEFINE_ENUM_OP_AND(T, U) \
    inline constexpr U operator&(T lhs, T rhs) \
    { \
        return static_cast<U>(lhs) & static_cast<U>(rhs); \
    }

#define SL_DEFINE_ENUM_OP_AND_EQUAL(T, U) \
    inline constexpr T operator&=(T lhs, T rhs) \
    { \
        return static_cast<T>(static_cast<U>(lhs) & static_cast<U>(rhs)); \
    }

#define SL_DEFINE_ENUM_OP_OR(T, U) \
    inline constexpr T operator|(T lhs, T rhs) \
    { \
        return static_cast<T>(static_cast<U>(lhs) | static_cast<U>(rhs)); \
    }

#define SL_DEFINE_ENUM_OP_OR_EQUAL(T, U) \
    inline constexpr T operator|=(T lhs, T rhs) \
    { \
        return static_cast<T>(static_cast<U>(lhs) | static_cast<U>(rhs)); \
    }

#define SL_DEFINE_BITWISE_OPERATION(T, U) \
    SL_DEFINE_ENUM_OP_AND(T, U) \
    SL_DEFINE_ENUM_OP_AND_EQUAL(T, U) \
    SL_DEFINE_ENUM_OP_OR(T, U) \
    SL_DEFINE_ENUM_OP_OR_EQUAL(T, U)

}
