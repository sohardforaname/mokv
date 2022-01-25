//
// Created by ucchi_mchxyz on 2021/12/5.
//

#include "arena.tcc"
#include "math.tcc"

namespace DB {

    void Arena::newBlock() {
        blocks_.emplace_back((char *) malloc(ARENA_PER_PAGE_SPACE));
        cur_block_ptr_ = blocks_.back();
        cur_block_remain_ = ARENA_PER_PAGE_SPACE;
    }

    Arena::~Arena() {
        for (auto ptr: blocks_) {
            free(ptr);
        }
        for (auto ptr: huge_blocks_) {
            free(ptr);
        }
    }

    void *Arena::hugeBlockAllocate(size_t alloc_size) {
        huge_blocks_.emplace_back((char *) malloc(alloc_size));
        huge_block_size_ += alloc_size;
        return huge_blocks_.back();
    }

    void *Arena::allocate(size_t alloc_size) {
        if (alloc_size > ARENA_PER_PAGE_SPACE) {
            return hugeBlockAllocate(alloc_size);
        }

        if (cur_block_remain_ < alloc_size) {
            newBlock();
        }

        auto ptr = cur_block_ptr_;
        cur_block_remain_ -= alloc_size;
        cur_block_ptr_ += alloc_size;
        return ptr;
    }

    void *Arena::alignedAllocate(size_t alloc_size, size_t *real_alloc_size) {
        auto align_size = sizeof(void *) - 1;

        auto aligned_alloc_size = getNextAlignedNum(alloc_size, align_size);
        if (aligned_alloc_size > ARENA_PER_PAGE_SPACE) {
            return hugeBlockAllocate(aligned_alloc_size);
        }

        auto aligned_ptr = (char *) getNextAlignedNum(size_t(cur_block_ptr_), align_size);

        cur_block_remain_ -= aligned_ptr - cur_block_ptr_;
        cur_block_ptr_ = aligned_ptr;

        if (nullptr != real_alloc_size) {
            *real_alloc_size = aligned_alloc_size;
        }

        return allocate(aligned_alloc_size);
    }

    size_t Arena::allocatedSize() const {
        return ARENA_PER_PAGE_SPACE * (blocks_.size() - 1) + (cur_block_ptr_ - blocks_.back()) +
               huge_block_size_;
    }
}