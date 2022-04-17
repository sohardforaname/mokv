#include "../util/const.tcc"
#include "file_meta.tcc"
#include <fcntl.h>
#include <mutex>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

namespace MOKV {

class FileCache {
private:
    std::unordered_map<int, FileMeta> file_map_;
    std::unordered_map<int, const char*> file_cache;

    std::vector<int> level_[SSTABLE_MAX_LEVEL];
    // std::mutex mutex_;

public:
    int openFile(const char* path);

    int getFileLevel(int fd);

    const std::vector<int>& getFilesOnLevel(int level);

    const FileMeta& getMeta(int fd);

    int findIndex(int fd, const char* key);
};

class FileIterator {
private:
    size_t len_, id_ = 0, kv_size_;
    size_t offset_ = 0, item_len_ = 0;
    const char* buffer_ = nullptr;

public:
    int setMetaInfo(const FileMeta& meta)
    {
        len_ = meta.getLen();
        kv_size_ = meta.kvSize();
        buffer_ = meta.getBuffer();
        return 0;
    }

    void seek(const char* value);

    void seekN(int n);

    size_t getId() const { return id_; }

    size_t diff() const { return offset_; }

    bool hasNext() const { return buffer_ && id_ != kv_size_; }

    void next();

    const std::string_view getItem();
};
}