//
// Created by ucchi_mchxyz on 2022/1/7.
//

#ifndef MOKV_BLOCK_BUILDER_TCC
#define MOKV_BLOCK_BUILDER_TCC

#include <string>
#include <cstring>
#include <vector>
#include <utility>
#include <unistd.h>
#include "../util/const.tcc"
#include "../util/bloom_filter.tcc"
#include "../db/schema.tcc"
#include "../util/comparator.tcc"

// TODO: key-value分离

namespace DB {

    struct FlexibleComparator : public Comparator<FlexibleComparator> {
        static int compare(const char *a, const char *b) {
            return memcmp(a + sizeof(size_t),
                          b + sizeof(size_t),
                          std::min(*(size_t *) a, *(size_t *) b));
        }
    };

    struct StrComparator : public Comparator<StrComparator> {
        size_t size_;

        StrComparator() = delete;

        explicit StrComparator(size_t size) : size_(size) {}

        int compare(const char *a, const char *b) {
            return memcmp(a, b, size_);
        }
    };

    class MetaBlockBuilder {
    private:
        size_t counter_;
        BloomFilter filter_;
        const char *max_data_ = nullptr, *min_data_ = nullptr;

        bool dumpHeaderBlock(int fd);

        bool dumpFilterBlock(int fd);

        Comparator<FlexibleComparator> *f_cmp_ = new FlexibleComparator;
        Comparator<StrComparator> *str_cmp_ = new StrComparator(0);

    public:
        MetaBlockBuilder() = default;

        MetaBlockBuilder(const MetaBlockBuilder &) = delete;

        MetaBlockBuilder(MetaBlockBuilder &&) = delete;

        ~MetaBlockBuilder() {
            delete f_cmp_;
            delete str_cmp_;
        }

        void addData(const char *data, size_t len, bool flexible = false);

        bool dumpMetaBlock(const char *file_path);
    };

    class DataBlockBuilder {
    private:
        // cache optimization
        const char *cached_data_[BLOCK_BUILDER_CACHE_SIZE] = {nullptr};
        size_t cache_size_ = 0;
        // end

        const Schema &schema_;
        size_t col_num_;

        struct Buffer_ {
            char *dat_ = (char *) malloc(BLOCK_BUILDER_INIT_SIZE);
            size_t size_ = 0, capacity_ = BLOCK_BUILDER_INIT_SIZE;
            MetaBlockBuilder meta_block_;

            Buffer_() = default;
        } *buffer_;

        size_t counter_ = 0;

        void reserve(size_t idx);

        void append();

    public:
        explicit DataBlockBuilder(const Schema &schema);

        DataBlockBuilder(const DataBlockBuilder &) = delete;

        DataBlockBuilder(DataBlockBuilder &&) = delete;

        void add(const char *data, size_t len);

        bool finish(const std::string &db_name);

        void reset();
    };
}


#endif //MOKV_BLOCK_BUILDER_TCC
