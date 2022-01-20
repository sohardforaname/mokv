//
// Created by ucchi_mchxyz on 2022/1/7.
//

#include <fcntl.h>
#include "block_builder.tcc"
#include "../util/defer.tcc"

namespace DB {

    bool MetaBlockBuilder::dumpHeaderBlock(int fd) {
        return false;
    }

    bool MetaBlockBuilder::dumpFilterBlock(int fd) {
        return false;
    }

    void MetaBlockBuilder::addData(const char *data, size_t len) {

    }

    bool MetaBlockBuilder::dumpMetaBlock(const char *file_path) {
        return false;
    }

    DataBlockBuilder::DataBlockBuilder(const Schema &schema)
            : schema_(schema),
              col_num_(schema.size()),
              buffer_(new Buffer_[schema.size()]) {}

    void DataBlockBuilder::reserve(size_t idx) {
        buffer_[idx].capacity_ <<= 1;
        buffer_[idx].dat_ = (char *) realloc(buffer_[idx].dat_, buffer_[idx].capacity_);
        if (nullptr == buffer_[idx].dat_) {
            throw std::bad_alloc{};
        }
    }

    void DataBlockBuilder::append() {
        for (size_t i = 0; i < col_num_; ++i) {
            for (size_t j = 0; j < cache_size_; ++j) {
                auto len = schema_.getColById(i).first;
                if (0 == len) {
                    len = *(size_t *) (cached_data_[j]) + sizeof(size_t);
                }

                if (buffer_[i].size_ + len > buffer_[i].capacity_) {
                    reserve(i);
                }
                memcpy(buffer_[i].dat_ + buffer_[i].size_, cached_data_[j], len);
                buffer_[i].size_ += len;
                cached_data_[j] += len;
            }
        }
        cache_size_ = 0;
    }

    void DataBlockBuilder::add(const char *data, size_t len) {
        cached_data_[cache_size_++] = data;
        ++counter_;
        if (cache_size_ == BLOCK_BUILDER_CACHE_SIZE) {
            append();
        }
    }

    bool DataBlockBuilder::finish(const std::string &db_name) {
        if (0 != cache_size_) {
            append();
        }

        auto path = "./" + db_name;
        if (-1 == mkdir(path.c_str())) {
            return false;
        }

        auto &col_name = schema_.getColName();
        for (size_t i = 0; i < col_num_; ++i) {
            auto fd = creat((path + "/" + col_name[i] + ".bin").c_str(), O_RDWR);
            if (-1 == fd) {
                return false;
            }
            Defer defer([&]() { return close(fd); });
            if (auto st = write(fd, buffer_[i].dat_, buffer_[i].size_); -1 == st) {
                return false;
            }
        }
        return true;
    }

    void DataBlockBuilder::reset() {
        for (size_t i = 0; i < col_num_; ++i) {
            buffer_[i].size_ = 0;
            buffer_[i].capacity_ = BLOCK_BUILDER_INIT_SIZE;
        }

        cache_size_ = 0;
    }
}