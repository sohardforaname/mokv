//
// Created by ucchi_mchxyz on 2021/12/21.
//

#ifndef MOKV_CONST_TCC
#define MOKV_CONST_TCC

const size_t ARENA_PER_PAGE_SPACE = 1 << 12;

const size_t MEMTABLE_MAX_SIZE = 1 << 28;
const size_t MEMTABLE_METADATA_SIZE = sizeof(size_t) * 4;

const size_t SKIP_LIST_LEVEL = 24;

const size_t BUFFER_POOL_PER_PAGE_SPACE = 1 << 12;
const size_t BUFFER_POOL_PAGE_COUNT = 16;

const size_t MAX_KEY_VALUE_LENGTH = 1 << 16;

const size_t BLOCK_BUILDER_INIT_SIZE = 1 << 12;
const size_t BLOCK_BUILDER_CACHE_SIZE = 1 << 4;
const size_t BLOOM_FILTER_SIZE = 1 << 10;

const size_t DB_MAX_IMMEMTABLE = 1;

#endif //MOKV_CONST_TCC