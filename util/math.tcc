//
// Created by ucchi_mchxyz on 2021/12/5.
//

#ifndef MOKV_MATH_TCC
#define MOKV_MATH_TCC

#include <cstddef>
#include <limits>

namespace MOKV {
template <class T>
constexpr T rightShift(T n, size_t bit)
{
    size_t k = 1;
    while (k < bit) {
        n |= n >> k;
        k <<= 1;
    }
    return n;
}

template <class T, size_t Bit = 64ULL>
T nextPowTwoSubOne(T n)
{
    return rightShift(n - 1, Bit);
}

template <class T>
T getNextAlignedNum(T n, T align_num)
{
    auto v = n & align_num;
    return rightShift(v, 64) ? n + ((~v) & align_num) + 1 : n;
}
}

#endif // MOKV_MATH_TCC