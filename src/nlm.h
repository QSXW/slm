#pragma once

#include "vector.h"

namespace nlm
{

inline float dot(const slm::Vector &u, const slm::Vector &v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

}
