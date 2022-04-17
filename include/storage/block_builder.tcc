//
// Created by ucchi_mchxyz on 2022/1/7.
//

#ifndef MOKV_BLOCK_BUILDER_TCC
#define MOKV_BLOCK_BUILDER_TCC

#include "../db/schema.tcc"
#include "../util/bloom_filter.tcc"
#include "../util/comparator.tcc"
#include "../util/const.tcc"
#include "../util/defer.tcc"
#include "util.tcc"
#include <cstring>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <utility>
#include <vector>

// TODO: key-value分离

namespace MOKV {

struct FlexibleComparator : public Comparator<FlexibleComparator> {
    static int compare(const char* a, const char* b)
    {
        return memcmp(a + sizeof(size_t),
            b + sizeof(size_t),
            std::min(*(size_t*)a, *(size_t*)b));
    }
};

struct StrComparator : public Comparator<StrComparator> {
    size_t size_;

    StrComparator() = delete;

    explicit StrComparator(size_t size)
        : size_(size)
    {
    }

    int compare(const char* a, const char* b)
    {
        return memcmp(a, b, size_);
    }
};

class MetaBlockBuilder {
private:
    BloomFilter filter_;
    size_t len_;

    size_t counter_ = 0;
    const char* buffer_addr_ = nullptr;
    size_t max_data_offset_ = -1, min_data_offset_ = -1;

    bool dumpHeaderBlock(int fd);

    bool dumpFilterBlock(int fd);

    Comparator<FlexibleComparator>* f_cmp_ = new FlexibleComparator;
    Comparator<StrComparator>* str_cmp_ = new StrComparator(0);

    template <class T>
    void setMinMax(Comparator<T>* comparator, const char* data, size_t offset);

public:
    MetaBlockBuilder(size_t len, const char* buffer_addr)
        : len_(len)
        , buffer_addr_(buffer_addr)
    {
    }

    MetaBlockBuilder(const MetaBlockBuilder&) = delete;

    MetaBlockBuilder(MetaBlockBuilder&&) = delete;

    ~MetaBlockBuilder()
    {
        delete f_cmp_;
        delete str_cmp_;
    }

    void resetBufferAddr(const char* new_addr)
    {
        buffer_addr_ = new_addr;
    }

    void addData(const char* data, size_t offset, size_t len);

    bool dumpMetaBlock(int fd);
};

class DataBlockBuilder {
private:
    // cache optimization
    const char* cached_data_[BLOCK_BUILDER_CACHE_SIZE] = { nullptr };
    size_t cache_size_ = 0;
    // end

    const Schema& schema_;
    size_t col_num_;

    struct Buffer_ {
        char* dat_ = (char*)malloc(BLOCK_BUILDER_INIT_SIZE);
        size_t size_ = 0, capacity_ = BLOCK_BUILDER_INIT_SIZE;
        MetaBlockBuilder* meta_block_ = nullptr;

        Buffer_() = default;

        ~Buffer_()
        {
            delete dat_;
            delete meta_block_;
        }
    } * buffer_;

    size_t counter_ = 0;

    void setFlexible();

    void reserve(size_t idx);

    void append();

public:
    explicit DataBlockBuilder(const Schema& schema);

    DataBlockBuilder(const DataBlockBuilder&) = delete;

    DataBlockBuilder(DataBlockBuilder&&) = delete;

    void add(const char* data, size_t len);

    int finish(const std::string& db_name, size_t id);

    void reset();
};
}

#endif // MOKV_BLOCK_BUILDER_TCC
