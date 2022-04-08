//
// Created by ucchi_mchxyz on 2021/12/5.
//

#ifndef MOKV_ARENA_TCC
#define MOKV_ARENA_TCC

#include "../util/const.tcc"
#include <vector>

namespace MOKV {

class Arena {
private:
    std::vector<char*> blocks_;
    size_t huge_block_size_ = 0;

    std::vector<char*> huge_blocks_;

    char* cur_block_ptr_ = nullptr;
    size_t cur_block_remain_ = 0;

    void newBlock();

    void* hugeBlockAllocate(size_t alloc_size);

public:
    Arena() { }

    ~Arena();

    void* allocate(size_t alloc_size);

    void* alignedAllocate(size_t alloc_size, size_t* real_alloc_size = nullptr);

    size_t allocatedSize() const;
};

}

#endif // MOKV_ARENA_TCC
