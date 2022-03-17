//
// Created by ucchi_mchxyz on 2022/1/27.
//

#include "file_reader.tcc"

namespace DB {

bool FileReader::open(const char* file_path)
{
    fd_ = buffer_pool_.openFile(file_path);
    if (-1 == fd_) {
        return false;
    }
    return true;
}

bool FileReader::loadMeta()
{
    auto p_id = buffer_pool_.getPage(fd_, 0);
    if (-1 == p_id) {
        return false;
    }
    buffer_ = buffer_pool_.getData(p_id);
    cur_offset_ = readFromBuffer(buffer_ + sizeof(size_t), count_, len_)
        + sizeof(size_t);
    auto min_max_len = len_ << 1;
    if (!len_) {
        auto min_size = *(size_t*)(buffer_ + cur_offset_);
        auto max_size = *(size_t*)(buffer_ + cur_offset_ + min_size + sizeof(size_t));
        min_max_len = min_size + max_size + (sizeof(size_t) << 1);
    }
    min_max_ = std::string(buffer_ + cur_offset_, min_max_len);
    return true;
}

FileIterator FileReader::iter()
{
    return FileIterator(this);
}

BloomFilter FileReader::loadBloomFilter()
{
    return BloomFilter();
}

size_t FileReader::kvSize()
{
    return count_;
}

const std::string& FileReader::getMinMax()
{
    return min_max_;
}
}
