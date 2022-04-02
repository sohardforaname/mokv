//
// Created by ucchi_mchxyz on 2021/12/1.
//

#include "memtable.tcc"

namespace DB {

size_t MemTable::sequence_num_ = 0;

void MemTable::insert(const char* key, const char* value, ValueType value_type)
{
    auto key_size = strlen(key), value_size = strlen(value);
    InternalKey internal_key(value_type, ++sequence_num_, key_size, value_size);

    size_t buffer_size = internal_key.internalKeySize() + key_size + value_size + sizeof(size_t);
    size_t aligned_buffer_size;

    auto buffer = (char*)record_pool_.alignedAllocate(buffer_size, &aligned_buffer_size);

    auto offset = writeToBuffer(buffer, aligned_buffer_size);
    offset += internal_key.generateInternalKey(buffer + offset);

    writeToBuffer(buffer + offset, key_size, std::make_pair(key, key_size), value_size,
        std::make_pair(value, value_size));
    // TODO: append crc32 code.
    table_.insert(buffer);
}

const char* MemTable::find(const char* key)
{
    auto node = table_.find(key);
    if (node.isValid()) {
        return node.dat();
    }
    return nullptr;
}

bool MemTable::generateSSTableFile(const char* file_path, const Schema& schema)
{
    DataBlockBuilder builder(schema);

    auto iter = table_.iter();
    while (iter.hasNext()) {
        auto ptr = iter.next();
        const auto offset = sizeof(size_t) * 3;
        builder.add(ptr + offset, *(size_t*)(ptr)-offset);
    }

    int res = builder.finish(file_path);
    builder.reset();
    return res;
}
}