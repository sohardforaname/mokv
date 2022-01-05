//
// Created by ucchi_mchxyz on 2021/12/12.
//

#include "buffer_pool.tcc"

namespace DB {
    BufferPool *BufferPool::instance_;

    BufferPool::BufferPool() : cache_(BUFFER_POOL_PAGE_COUNT) {
        for (auto i = 0; i < BUFFER_POOL_PAGE_COUNT; ++i) {
            frame_buffer_[i].id_ = i;
            free_list_.emplace_back(frame_buffer_ + i);
        }
    }

    BufferPool::~BufferPool() {

    }

    size_t BufferPool::openFile(const char *file_path) {
        FILE *file_ptr = fopen(file_path, "ab+");
        if (nullptr == file_ptr) {
            return -1;
        }

        static size_t file_no;

        while (file_map_.find(file_no) != file_map_.end()) {
            ++file_no;
        }
        file_map_[file_no] = file_ptr;
        return file_no;
    }

    FrameID BufferPool::getPage(size_t file_no, size_t offset) {
        if (auto page_node = page_map_.find(file_no << 32 | offset); page_map_.end() != page_node) {
            return page_node->second;
        }

        auto file_node = file_map_.find(file_no);
        if (file_map_.end() == file_node) {
            return -1;
        }

        auto page = free_list_.front();
        free_list_.pop_front();

        page->file_no_ = file_no;
        page->file_offset_ = offset;

        page_map_.insert({file_no << 32 | offset, page->id_});
        cache_.put(page->id_, page);

        FILE* file_ptr = file_node->second;
        fseek(file_ptr, offset, SEEK_SET);
        fread(page->buffer_, sizeof(char), 4096, file_ptr);

        return page->id_;
    }

    const char *BufferPool::getData(FrameID frame_id) {
        return frameIdToFramePtr(frame_id)->buffer_;
    }

    void BufferPool::pinPage(FrameID frame_id) {
        cache_.erase(frame_id);
        pin_map_.insert(frameIdToFramePtr(frame_id));
    }

    void BufferPool::unpinPage(FrameID frame_id) {
        auto ptr = frameIdToFramePtr(frame_id);
        free_list_.emplace_back(ptr);
        pin_map_.erase(ptr);
    }

    void BufferPool::closeFile(size_t file_no) {
        for (auto iter : page_map_) {
            if ((iter.first >> 32) == file_no) {
                cache_.erase(iter.second);
            }
        }
        auto iter = pin_map_.begin();
        while (iter != pin_map_.end()) {
            if ((*iter)->file_no_ == file_no) {
                iter = pin_map_.erase(iter);
                continue;
            }
            ++iter;
        }
        file_map_.erase(file_no);
    }
}