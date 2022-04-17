//
// Created by ucchi_mchxyz on 2022/1/7.
//

#include "block_builder.tcc"

namespace MOKV {

bool MetaBlockBuilder::dumpHeaderBlock(int fd)
{
    write(fd, &SSTABLE_MAGIC_NUM, sizeof(size_t));
    write(fd, &counter_, sizeof(size_t));
    write(fd, &len_, sizeof(size_t));

    size_t min_data_len = len_ ? len_ : (*(size_t*)(buffer_addr_ + min_data_offset_)) + sizeof(size_t),
           max_data_len = len_ ? len_ : (*(size_t*)(buffer_addr_ + max_data_offset_)) + sizeof(size_t);
    write(fd, buffer_addr_ + min_data_offset_, min_data_len);
    write(fd, buffer_addr_ + max_data_offset_, max_data_len);
    return true;
}

bool MetaBlockBuilder::dumpFilterBlock(int fd)
{
    // write(fd, filter_.buffer(), BLOOM_FILTER_SIZE);
    return true;
}

template <class T>
void MetaBlockBuilder::setMinMax(Comparator<T>* comparator, const char* data, size_t offset)
{
    int res = comparator->compare(data, buffer_addr_ + min_data_offset_);
    if (res < 0) {
        min_data_offset_ = offset;
    }
    res = comparator->compare(data, buffer_addr_ + max_data_offset_);
    if (res > 0) {
        max_data_offset_ = offset;
    }
}

void MetaBlockBuilder::addData(const char* data, size_t offset, size_t len)
{
    ++counter_;
    filter_.set(data, len);
    if (-1 == max_data_offset_) {
        max_data_offset_ = min_data_offset_ = offset;
        return;
    }
    if (!len_) {
        setMinMax(f_cmp_, data, offset);
        return;
    }
    str_cmp_ = new (str_cmp_) StrComparator(len);
    setMinMax(str_cmp_, data, offset);
}

bool MetaBlockBuilder::dumpMetaBlock(int fd)
{
    return dumpHeaderBlock(fd) && dumpFilterBlock(fd);
}

DataBlockBuilder::DataBlockBuilder(const Schema& schema)
    : schema_(schema)
    , col_num_(schema.size())
    , buffer_(new Buffer_[schema.size()])
{
    setFlexible();
}

void DataBlockBuilder::reserve(size_t idx)
{
    buffer_[idx].capacity_ <<= 1;
    buffer_[idx].dat_ = (char*)realloc(buffer_[idx].dat_, buffer_[idx].capacity_);
    if (nullptr == buffer_[idx].dat_) {
        throw std::bad_alloc {};
    }
    buffer_[idx].meta_block_->resetBufferAddr(buffer_[idx].dat_);
}

void DataBlockBuilder::append()
{
    for (size_t i = 0; i < col_num_; ++i) {
        for (size_t j = 0; j < cache_size_; ++j) {
            auto len = schema_.getColById(i).first;
            if (0 == len) {
                len = *(size_t*)(cached_data_[j]) + sizeof(size_t);
            }

            if (buffer_[i].size_ + len > buffer_[i].capacity_) {
                reserve(i);
            }
            buffer_[i].meta_block_->addData(cached_data_[j], buffer_[i].size_, len);
            memcpy(buffer_[i].dat_ + buffer_[i].size_, cached_data_[j], len);
            buffer_[i].size_ += len;
            cached_data_[j] += len;
        }
    }
    cache_size_ = 0;
}

void DataBlockBuilder::setFlexible()
{
    auto lens = schema_.getColLen();
    for (size_t i = 0; i < lens.size(); ++i) {
        buffer_[i].meta_block_ = new MetaBlockBuilder(lens[i], buffer_[i].dat_);
    }
}

void DataBlockBuilder::add(const char* data, size_t len)
{
    cached_data_[cache_size_++] = data;
    ++counter_;
    if (cache_size_ == BLOCK_BUILDER_CACHE_SIZE) {
        append();
    }
}

// The size of memtable is determined.
// so the size of the SSTable will not be too big
// and can be written to a file.

int DataBlockBuilder::finish(const std::string& db_name, size_t id)
{
    if (0 != cache_size_) {
        append();
    }

    auto path = "./" + db_name;
    umask(0);
    if (-1 == access(path.c_str(), F_OK)) {
        return errno;
    }

    auto& col_name = schema_.getColName();
    for (size_t i = 0; i < col_num_; ++i) {
        // auto file_name = std::move(path + "/" + col_name[i] + "_" + std::to_string(id) + "_0.sst");
        auto file_name = std::move(generateFileName(db_name, col_name[i], id, 0));
        auto fd = open(file_name.c_str(), O_CREAT | O_WRONLY, 0666);
        if (-1 == fd) {
            return errno;
        }
        Defer defer([&]() { close(fd); });
        if (auto st = buffer_[i].meta_block_->dumpMetaBlock(fd); !st) {
            return -1;
        }
        if (auto st = write(fd, buffer_[i].dat_, buffer_[i].size_); - 1 == st) {
            return errno;
        }
    }
    return 0;
}

void DataBlockBuilder::reset()
{
    for (size_t i = 0; i < col_num_; ++i) {
        buffer_[i].size_ = 0;
        buffer_[i].capacity_ = BLOCK_BUILDER_INIT_SIZE;
    }

    cache_size_ = 0;
}
}