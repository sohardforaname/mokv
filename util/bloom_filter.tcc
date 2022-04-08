//
// Created by ucchi_mchxyz on 2022/1/21.
//

#ifndef MOKV_BLOOM_FILTER_TCC
#define MOKV_BLOOM_FILTER_TCC

#include "const.tcc"
#include <array>
#include <cstring>

namespace MOKV {

static const std::array<size_t, 8> hash_args = {
    17, 31, 131, 137,
    65599, 998244353, 1000000007, 1000000009
};

class BloomFilter {
private:
    char buffer_[BLOOM_FILTER_SIZE] = { 0 };

    size_t hash(const char* str, size_t len, size_t seed)
    {
        size_t val = 0;
        while (len--) {
            val = val * seed + (*str++);
        }
        return val;
    }

public:
    void set(const char* str, size_t len)
    {
        for (auto v : hash_args) {
            auto val = hash(str, len, v) & (BLOOM_FILTER_SIZE - 1);
            buffer_[val / 8] |= 1 << (val % 8);
        }
    }

    bool check(const char* str, size_t len)
    {
        for (auto v : hash_args) {
            auto val = hash(str, len, v) & (BLOOM_FILTER_SIZE - 1);
            if (0 == ((buffer_[val / 8] >> (val % 8)) & 1)) {
                return false;
            }
        }
        return true;
    }

    void reset()
    {
        memset(buffer_, 0, sizeof(buffer_));
    }

    const char* buffer() const
    {
        return buffer_;
    }
};
}

#endif // MOKV_BLOOM_FILTER_TCC
