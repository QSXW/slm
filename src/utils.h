#pragma once

#include "utils.h"

namespace slm
{

template <class T>
inline constexpr const T &max(const T &x, const T &y)
{
    return std::max(x, y);
}

template <class T>
inline constexpr const T &min(const T &x, const T &y)
{
    return std::min(x, y);
}

template <class T>
inline constexpr const T &clamp(const T &v, const T &min_v, const T &max_v)
{
    return std::max(std::min(v, max_v), min_v);
}

}
