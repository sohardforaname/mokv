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
#include "../db/schema.tcc"

// TODO: key-value分离

namespace DB {

    class MetaBlockBuilder {
    private:
        size_t counter_;
        char filter_[BLOOM_FILTER_SIZE];
        const char *max_data, *min_data;

        bool dumpHeaderBlock(int fd);

        bool dumpFilterBlock(int fd);

    public:
        MetaBlockBuilder() = default;

        MetaBlockBuilder(const MetaBlockBuilder &) = delete;

        MetaBlockBuilder(MetaBlockBuilder &&) = delete;

        void addData(const char *data, size_t len);

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
