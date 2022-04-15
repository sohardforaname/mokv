//
// Created by ucchi_mchxyz on 2021/12/1.
//

#ifndef MOKV_MEMTABLE_TCC
#define MOKV_MEMTABLE_TCC

#include "../util/binary.tcc"
#include "../util/const.tcc"
#include "../util/math.tcc"
#include "block_builder.tcc"
#include "skip_list.tcc"
#include <cstdio>
#include <cstring>
#include <functional>
#include <memory>

namespace MOKV {

enum ValueType {
    Undefined,
    Value,
    Deletion
};

struct InternalKeyComparator {
    static int compare(const char* a, const char* b)
    {
        const size_t KEY_SIZE_OFFSET = sizeof(size_t) * 3;
        return memcmp(a + MEMTABLE_METADATA_SIZE,
            b + MEMTABLE_METADATA_SIZE,
            std::min(*(size_t*)(a + KEY_SIZE_OFFSET), *(size_t*)(b + KEY_SIZE_OFFSET)));
    }
};

class MemTable {
private:
    // format: data_size | value_type | sequence_num | key_size | key | value_size | value |
    class InternalKey {
        size_t value_type_ = Undefined;
        size_t sequence_num_;

        size_t key_size_, value_size_;

        InternalKey() = default;

    public:
        InternalKey(ValueType value_type, size_t sequence_num, size_t key_size, size_t value_size)
            : value_type_(value_type)
            , sequence_num_(sequence_num)
            , key_size_(key_size)
            , value_size_(value_size)
        {
        }

        size_t generateInternalKey(char* buffer)
        {
            return writeToBuffer(buffer, value_type_, sequence_num_);
        }

        constexpr size_t internalKeySize() const
        {
            return sizeof(value_type_) + sizeof(key_size_) + sizeof(value_size_) + sizeof(sequence_num_);
        }
    };

    SkipList<const char*, InternalKeyComparator> table_;

    static size_t sequence_num_;

    Arena record_pool_;

    bool is_full_ = false;

    size_t ref_ = 0;

public:
    MemTable() = default;

    bool generateSSTableFile(const char* file_path, const Schema& schema, size_t id);

    void insert(const char* key, size_t key_size,
        const char* value, size_t value_size,
        ValueType value_type);

    const char* find(const char* key);

    bool isFull() const
    {
        return is_full_;
    }

    static void ref(MemTable* mem)
    {
        ++mem->ref_;
    }

    static void unref(MemTable* mem)
    {
        --mem->ref_;
        if (mem->ref_ <= 0) {
            delete mem;
        }
    }
};

}

#endif // MOKV_MEMTABLE_TCC
