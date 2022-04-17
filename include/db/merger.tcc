//
// Created by ucchi_mchxyz on 2022/1/9.
//

#ifndef MOKV_MERGER_TCC
#define MOKV_MERGER_TCC

#include <cstddef>
#include <cstdio>

namespace MOKV {

class MergeIterator {
private:
    FILE* sst_file;

public:
    explicit MergeIterator(FILE* sst_file);

    const char* next();

    bool valid() const;

    size_t size() const;
};

}

#endif // MOKV_MERGER_TCC
