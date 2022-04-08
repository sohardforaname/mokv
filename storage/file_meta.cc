//
// Created by ucchi_mchxyz on 2022/1/27.
//

#include "file_meta.tcc"

namespace MOKV {

int FileMeta::loadMeta()
{
    size_t magic_num;
    readFromBinaryFile(fd_, magic_num);
    if (SSTABLE_MAGIC_NUM != magic_num) {
        return -1;
    }
    readFromBinaryFile(fd_, count_, len_);
    if (0 == len_) {
        size_t len;
        const char* p1 = loadVariableLengthString(fd_, len);
        min_ = std::string_view(p1, len);
        const char* p2 = loadVariableLengthString(fd_, len);
        max_ = std::string_view(p2, len);
        return errno;
    }
    char *min_buffer = new char[len_], *max_buffer = new char[len_];
    readFromBinaryFile(fd_, std::make_pair(min_buffer, len_), std::make_pair(max_buffer, len_));
    min_ = std::string_view(min_buffer);
    max_ = std::string_view(max_buffer);
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
        return -1;
    }
    path_ = file_path;
    return 0;
}

const BloomFilter& FileMeta::getBloomFilter()
{
    return filter_;
}

const char* FileMeta::getBuffer()
{
    if (buffer_ != nullptr) {
        return buffer_;
    }

    struct stat file_info;
    if (stat(path_, &file_info)) {
        return nullptr;
    }
    int cur_offset = lseek(fd_, 0, SEEK_CUR);
    if (-1 == cur_offset) {
        return nullptr;
    }
    int data_size = file_info.st_size - cur_offset;
    buffer_ = new char[data_size];
    if (-1 == ::read(fd_, buffer_, data_size)) {
        delete[] buffer_;
        buffer_ = nullptr;
        return nullptr;
    }
    return buffer_;
}

std::pair<const std::string_view, const std::string_view> FileMeta::getMinMax() const
{
    return make_pair(min_, max_);
}
}