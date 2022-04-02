//
// Created by ucchi_mchxyz on 2022/1/27.
//

#ifndef MOKV_FILE_READER_TCC
#define MOKV_FILE_READER_TCC

#include "../util/binary.tcc"
#include "../util/bloom_filter.tcc"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

namespace DB {

class FileMeta {

    const char* buffer_;
    const char* path;
    int fd_;
    int file_type_; // 0 for random access file，1 for sequencial access file.

    size_t len_ = 0, count_ = 0;
    BloomFilter filter_;

    std::string_view min_, max_;

    int loadMeta();

    int loadBloomfilter();

public:
    int openFile(const char* file_path);

    const BloomFilter& getBloomFilter();

    size_t kvSize();

    std::pair<const std::string_view, const std::string_view> getMinMax();
};
}

#endif // MOKV_FILE_READER_TCC
