//
// Created by ucchi_mchxyz on 2022/1/27.
//

#ifndef MOKV_FILE_META_TCC
#define MOKV_FILE_META_TCC

#include "../util/binary.tcc"
#include "../util/bloom_filter.tcc"
#include "../util/slice.tcc"
#include <fcntl.h>
#include <memory>
#include <sys/stat.h>
#include <unistd.h>

namespace MOKV {

class FileMeta {

    mutable AllocSlice buffer_;
    std::string path_;
    int fd_;
    int file_type_; // 0 for random access file，1 for sequencial access file.

    size_t len_ = 0, count_ = 0;
    BloomFilter filter_;

    AllocSlice min_, max_;

    int loadMeta();

    int loadBloomfilter();

public:
    int openFile(const char* file_path);

    const BloomFilter& getBloomFilter();

    size_t kvSize() const { return count_; }

    int fd() const { return fd_; }

    void closeFd() { close(fd_); }

    const char* getBuffer() const;

    size_t getLen() const { return len_; }

    const char* getPath() const { return path_.c_str(); };

    std::pair<const std::string_view, const std::string_view> getMinMax() const;
};
}

#endif // MOKV_FILE_READER_TCC
