#include "file_cache.tcc"

namespace MOKV {
int FileCache::openFile(const char* path)
{
    FileMeta meta;
    int status = meta.openFile(path);
    if (status) {
        return -1;
    }
    file_map_.emplace(meta.fd(), std::move(meta));
    return meta.fd();
}

int FileCache::getFileLevel(int fd)
{
    for (int i = 0; i < SSTABLE_MAX_LEVEL; ++i) {
        for (auto fd_ : level_[i]) {
            if (fd_ == fd) {
                return i;
            }
        }
    }
    return -1;
}

const std::vector<int>& FileCache::getFilesOnLevel(int level)
{
    return level_[level];
}

const FileMeta& FileCache::getMeta(int fd)
{
    return file_map_[fd];
}

int FileCache::findIndex(int fd, const char* key)
{
    FileIterator iter;
    if (iter.setMetaInfo(file_map_[fd])) {
        return 0;
    }
    iter.seek(key);
    return iter.hasNext() ? iter.getId() : 0;
}

void FileIterator::seek(const char* value)
{
    std::string_view value_view(value);
    while (hasNext()) {
        next();
        auto view = getItem();
        if (view == value_view) {
            return;
        }
    }
}

void FileIterator::seekN(int n)
{
    while (hasNext() && n--) {
        next();
    }
}

void FileIterator::next()
{
    id_++;
    offset_ += item_len_;
    item_len_ = len_;
    if (len_ == 0) {
        readFromBuffer(buffer_ + offset_, item_len_);
        offset_ += sizeof(len_);
    }
}

const std::string_view FileIterator::getItem()
{
    return std::string_view(buffer_ + offset_, item_len_);
}

}