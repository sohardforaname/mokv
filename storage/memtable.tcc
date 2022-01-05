//
// Created by ucchi_mchxyz on 2021/12/1.
//

#ifndef MOKV_MEMTABLE_TCC
#define MOKV_MEMTABLE_TCC

#include <cstring>
#include <functional>
#include <memory>
#include <cstdio>
#include "skip_list.tcc"
#include "../util/binary.tcc"
#include "../util/math.tcc"
#include "../util/const.tcc"

namespace DB {

    enum ValueType {
        Undefined,
        Value,
        Deletion
    };

    class MemTable {
    private:
        // format: data_size | value_type | sequence_num | key_size | value_size | key | value | crc32(optional) |
        class InternalKey {
            size_t value_type_ = Undefined;
            size_t sequence_num_;

            size_t key_size_, value_size_;

            InternalKey() = default;

        public:
            InternalKey(ValueType value_type, size_t sequence_num, size_t key_size, size_t value_size)
                    : value_type_(value_type),
                      sequence_num_(sequence_num),
                      key_size_(key_size),
                      value_size_(value_size) {}

            size_t generateInternalKey(char *buffer) {
                return writeToBuffer(buffer, value_type_, sequence_num_, key_size_, value_size_);
            }

            constexpr size_t internalKeySize() const {
                return sizeof(value_type_) + sizeof(key_size_) + sizeof(value_size_) +
                       sizeof(sequence_num_);
            }
        };

        struct InternalKeyComparator {
            static int compare(const char *a, const char *b) {
                return strcmp(a + MEMTABLE_METADATA_SIZE, b + MEMTABLE_METADATA_SIZE);
            }
        };

        SkipList<const char *, InternalKeyComparator> table_;

        static size_t sequence_num_;

        Arena record_pool_;

        bool generateSSTableFile(const char *file_path);

    public:
        void insert(const char *key, const char *value, ValueType value_type);

        const char *find(const char *key);

        size_t scan(std::function<bool(const char *)>);
    };

}

#endif //MOKV_MEMTABLE_TCC
