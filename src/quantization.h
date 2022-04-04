#include "slm.h"

namespace slm
{

static inline void dequantize_uint16_64(uint16_t *coef, int16_t *table)
{
    int16x32 c1{ coef  };
    int16x32 t1{ table };
    int16x32 c2{ coef  + 32 };
    int16x32 t2{ table + 32 };

    c1 = c1 * t1;
    c2 = c2 * t2;

    c1.store(coef);
    c2.store(coef + 32);
}

}
