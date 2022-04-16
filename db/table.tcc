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
#include "../storage/util.tcc"
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

    // std::mutex mutex_;

    FileCache files_;

    size_t line_id_;

    int schema_fd_;

    Table() = delete;

    Table(std::string name, Schema schema, int schema_fd,
        size_t l0_sst_id = 0, size_t line_id = 0)
        : name_(std::move(name))
        , schema_(std::move(schema))
        , schema_fd_(schema_fd)
        , l0_sst_id_(l0_sst_id)
        , line_id_(line_id)
    {
    }

    friend TableSet;

    void findDataInMemtable();

    void findDataInSSTable();

    std::mutex mutex_;

public:
    Table(const Table&) = delete;

    const std::string_view name() const { return name_; }

    const Schema& schema() const { return schema_; }

    void write(const char* key, const char* value);

    void writeBatch(Batch batch);

    int findIndex(
        const std::string& col_name,
        std::vector<int>& indics,
        bool (*condition)(const std::string_view));

    int findData(
        const std::string& col_name,
        std::vector<std::string>& data,
        bool (*condition)(const std::string_view));

    int findAll(const std::string& col_name,
        std::vector<std::string>& data,
        std::vector<int>& indics,
        bool (*condition)(const std::string_view),
        int mode = 3);

    RangeIterator range();

    int close();
};

class TableSet {
private:
    std::unordered_map<std::string, Table*> tables_;

public:
    Table* open(std::string db_name);

    Table* create(std::string db_name, Schema schema);

    size_t size() const { return tables_.size(); }

    int close(std::string db_name);
};

}

#endif // MOKV_TABLE_TCC
