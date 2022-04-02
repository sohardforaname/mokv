//
// Created by ucchi_mchxyz on 2022/1/27.
//

#include "file_meta.tcc"

namespace DB {

const char* loadVariableLengthString(int fd, int& len)
{
    int vlen;
    readFromBinaryFile(fd, vlen);
    len = vlen;
    char* buffer = new char[vlen];
    readFromBinaryFile(fd, std::make_pair(buffer, vlen));
    return buffer;
}

int FileMeta::loadMeta()
{
    size_t magic_num;
    readFromBinaryFile(fd_, magic_num);
    if (SSTABLE_MAGIC_NUM != magic_num) {
        return -1;
    }
    readFromBinaryFile(fd_, count_, len_);
    if (0 == len_) {
        int len;
        min_ = std::move(std::string_view(loadVariableLengthString(fd_, len), len));
        max_ = std::move(std::string_view(loadVariableLengthString(fd_, len), len));
        return errno;
    }
    char *min_buffer = new char[len_], *max_buffer = new char[len_];
    readFromBinaryFile(fd_, std::make_pair(min_buffer, len_), std::make_pair(max_buffer, len_));
    min_ = std::move(std::string_view(min_buffer, len_));
    max_ = std::move(std::string_view(max_buffer, len_));
    return errno;
}

int FileMeta::loadBloomfilter()
{
    return 0;
}

int FileMeta::openFile(const char* file_path)
{
    fd_ = ::open(file_path, O_RDONLY);
    if (fd_ == -1 || loadMeta() || loadBloomfilter()) {
        return errno;
    }
    return 0;
}

const BloomFilter& FileMeta::getBloomFilter()
{
    return filter_;
}

size_t FileMeta::kvSize()
{
    return count_;
}

std::pair<const std::string_view, const std::string_view> FileMeta::getMinMax()
{
    return make_pair(std::string_view(min_), std::string_view(max_));
}
}