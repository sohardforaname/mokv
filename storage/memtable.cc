//
// Created by ucchi_mchxyz on 2021/12/1.
//

#include "memtable.tcc"

namespace DB {

    size_t MemTable::sequence_num_ = 0;

    void MemTable::insert(const char *key, const char *value, ValueType value_type) {
        auto key_size = strlen(key), value_size = strlen(value);
        InternalKey internal_key(value_type, ++sequence_num_, key_size, value_size);

        size_t buffer_size = internal_key.internalKeySize() + key_size + value_size + sizeof(size_t);
        size_t aligned_buffer_size;

        auto buffer = (char *) record_pool_.alignedAllocate(buffer_size, &aligned_buffer_size);

        auto offset = writeToBuffer(buffer, aligned_buffer_size);
        offset += internal_key.generateInternalKey(buffer + offset);

        writeToBuffer(buffer + offset, std::make_pair(key, key_size), std::make_pair(value, value_size));
        // TODO: append crc32 code.
        table_.insert(buffer);
    }

    const char *MemTable::find(const char *key) {
        auto node = table_.find(key);
        if (node.isValid()) {
            return node.dat();
        }
        return nullptr;
    }

    size_t MemTable::scan(std::function<bool(const char *)> scan_func) {
        return table_.scan(std::move(scan_func));
    }

    bool MemTable::generateSSTableFile(const char *file_path) {
        FILE *file = fopen(file_path, "wb");
        if (nullptr == file) {
            return false;
        }

        scan([&](const char *dat) -> bool {
            fwrite(dat, sizeof(char), *(size_t *) dat, file);
            return true;
        });

        return true;
    }
}