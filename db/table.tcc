//
// Created by ucchi_mchxyz on 2022/1/13.
//

#ifndef MOKV_TABLE_TCC
#define MOKV_TABLE_TCC

#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../storage/file_cache.tcc"
#include "../storage/memtable.tcc"
#include "../util/defer.tcc"
#include "range.tcc"

namespace MOKV {

class TableSet;

class Batch {
private:
    std::vector<std::pair<const char*, const char*>> dat_;

public:
    Batch() = default;

    Batch(const Batch&) = delete;

    Batch(Batch&&) = default;

    size_t size() const
    {
        return dat_.size();
    }

    const auto& dat() const
    {
        return dat_;
    }

    void put(const char* key, const char* value)
    {
        dat_.emplace_back(key, value);
    }
};

class Table {
private:
    std::string name_;
    Schema schema_;

    MemTable* mem_ = new MemTable;
    std::vector<MemTable*> imm_;

    size_t l0_sst_id_ = 0;

    // FileCache files_;

    Table() = delete;

    Table(std::string name, Schema schema)
        : name_(std::move(name))
        , schema_(std::move(schema))
    {
    }

    friend TableSet;

public:
    Table(const Table&) = delete;

    Table(Table&&) = default;

    const std::string_view name() const
    {
        return name_;
    }

    const Schema& schema() const
    {
        return schema_;
    }

    void write(const char* key, const char* value);

    void writeBatch(Batch batch);

    const char* find(const char* key, size_t col_idx);

    RangeIterator range();
};

class TableSet {
private:
    std::unordered_map<std::string, Table> tables_;

public:
    Table* create(std::string db_name, Schema schema)
    {
        auto path = std::move("./" + db_name);
        umask(0);
        if (-1 == mkdir(path.c_str(), 0777)) {
            return nullptr;
        }
        auto fd = open((path + "/" + db_name + ".sma").c_str(), O_CREAT | O_WRONLY, 0666);
        if (-1 == fd) {
            return nullptr;
        }
        Defer defer([&]() { close(fd); });
        auto schema_string = schema.toString();
        if (-1 == ::write(fd, schema_string.c_str(), schema_string.size())) {
            return nullptr;
        }

        return &tables_.try_emplace(std::move(db_name), std::move(Table(db_name, std::move(schema)))).first->second;
    }

    size_t size() const
    {
        return tables_.size();
    }
};

}

#endif // MOKV_TABLE_TCC
