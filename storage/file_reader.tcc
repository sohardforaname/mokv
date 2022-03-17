//
// Created by ucchi_mchxyz on 2022/1/27.
//

#ifndef MOKV_FILE_READER_TCC
#define MOKV_FILE_READER_TCC

#include <unistd.h>
#include "../util/bloom_filter.tcc"
#include "../util/binary.tcc"
#include "buffer_pool.tcc"

namespace DB {

    class FileReader;

    class FileIterator {
    private:
        FileReader* reader_;

        FileIterator(FileReader* reader) :
                reader_(reader) {}

        friend class FileReader;

    public:

        bool hasNext() {
            return false;
        }

        const char *next() {
            return nullptr;
        }
    };

    class FileReader {

        const char *buffer_;
        size_t fd_; 

        size_t len_ = 0, count_ = 0;
        size_t cur_offset_ = 0;

        BufferPool &buffer_pool_;
        std::string min_max_;

    public:
        FileReader() : buffer_pool_(BufferPool::getBufferPool()) {}

        ~FileReader() {}

        bool open(const char *file_path);

        bool loadMeta();

        FileIterator iter();

        BloomFilter loadBloomFilter();

        size_t kvSize();

        const std::string& getMinMax();
    };
}

#endif //MOKV_FILE_READER_TCC
