//
// Created by ucchi_mchxyz on 2021/12/12.
//

#ifndef MOKV_BUFFER_POOL_TCC
#define MOKV_BUFFER_POOL_TCC

#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <list>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include "../util/const.tcc"
#include "../util/lru_cache.tcc"

typedef size_t FrameID;

namespace DB {

    class BufferPool {
    private:
        struct Frame {
            size_t id_ = -1;
            size_t file_no_ = -1;
            size_t file_offset_ = -1;
            char *buffer_ = (char *) malloc(BUFFER_POOL_PER_PAGE_SPACE);
        };

        // resources.
        Frame *frame_buffer_ = new Frame[BUFFER_POOL_PAGE_COUNT];

        std::deque<Frame *> free_list_, flush_list_;
        std::unordered_set<Frame *> pin_map_;

        // lru cache
        LruCache<size_t, Frame> cache_;

        // index map
        std::unordered_map<size_t, FILE *> file_map_;
        std::unordered_map<size_t, size_t> page_map_;

        static BufferPool *instance_;

        struct Deleter {
            BufferPool *ptr_;

            Deleter() = delete;

            explicit Deleter(BufferPool *ptr) : ptr_(ptr) {}

            ~Deleter() {
                delete ptr_;
            }
        };

        static Deleter deleter;

        Frame *frameIdToFramePtr(FrameID frame_id) const {
            return frame_buffer_ + frame_id;
        }

        BufferPool();

        ~BufferPool();

    public:
        BufferPool &operator=(const BufferPool &) = delete;

        BufferPool(const BufferPool &) = delete;

        static BufferPool &getBufferPool() {
            static std::once_flag f;
            std::call_once(f, [&]() { instance_ = new BufferPool; });
            return *instance_;
        }

        size_t openFile(const char *file_path);

        FrameID getPage(size_t file_no, size_t offset);

        const char *getData(FrameID frame_id);

        void pinPage(FrameID frame_id);

        void unpinPage(FrameID frame_id);

        void closeFile(size_t file_no);
    };
}


#endif //MOKV_BUFFER_POOL_TCC
