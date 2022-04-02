#include "../util/const.tcc"
#include "file_meta.tcc"
#include <fcntl.h>
#include <mutex>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

namespace DB {

class FileCache {
private:
    std::unordered_map<int, FileMeta> file_map_;
    std::vector<int> level_[SST_MAX_LEVEL];
    std::mutex mutex_;

public:
    int openFile(const char* path);

    int getFileLevel(int fd);

    const std::vector<int>& getFilesOnLevel(int level);

    FileMeta& getMeta(int fd);
};
}