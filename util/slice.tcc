#include <cstring>
#include <memory>

namespace MOKV {

class AllocSlice {
private:
    std::shared_ptr<char[]> data_;

    size_t len_;

public:
    AllocSlice(const char* s, int len)
        : len_(len)
    {
        char* blk = new char[len];
        memcpy(blk, s, len);
        data_.reset(blk);
    }

    AllocSlice(const AllocSlice&) = default;
    AllocSlice(AllocSlice&&) = default;

    ~AllocSlice() = default;

    const char* data() const { return data_.get(); }
    size_t size() const { return len_; }

    int compare(const AllocSlice& rhs)
    {
        auto min_size = std::min(len_, rhs.len_);
        int res = memcmp(data_.get(), rhs.data_.get(), min_size);
        if (0 == res) {
            if (len_ < rhs.len_) {
                return -1;
            }
            if (len_ > rhs.len_) {
                return 1;
            }
        }
        return res;
    }
};
}