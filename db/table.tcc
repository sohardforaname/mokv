//
// Created by ucchi_mchxyz on 2022/1/13.
//

#ifndef MOKV_TABLE_TCC
#define MOKV_TABLE_TCC

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <thread>

#include "../storage/memtable.tcc"
#include "../storage/buffer_pool.tcc"
#include "range_iterator.tcc"

namespace DB {

    class TableSet;

    class Batch {
    private:
        std::vector<std::pair<const char *, const char *>> dat_;

    public:
        Batch() = default;

        Batch(const Batch &) = delete;

        Batch(Batch &&) = default;

        size_t size() const {
            return dat_.size();
        }

        const auto &dat() const {
            return dat_;
        }

        void put(const char *key, const char *value) {
            dat_.emplace_back(key, value);
        }
    };


    class Table {
    private:
        std::string name_;
        Schema schema_;

        MemTable *mem_ = new MemTable;
        std::vector<MemTable *> imm_;

        BufferPool &pool_ = BufferPool::getBufferPool();

        Table() = delete;

        Table(std::string name, Schema schema) : name_(std::move(name)),
                                                 schema_(std::move(schema)) {}

        friend TableSet;

    public:

        Table(const Table &) = delete;

        Table(Table &&) = default;

        std::string_view name() const {
            return name_;
        }

        const Schema &schema() const {
            return schema_;
        }

        void write(const char *key, const char *value);

        void writeBatch(Batch batch);

        const char *find(const char *key, size_t col_idx);

        RangeIterator range();

    };

    class TableSet {
    private:
        std::unordered_map<std::string, Table> tables_;

    public:
        Table *create(std::string name, Schema schema) {
            return &tables_.try_emplace(std::move(name), std::move(Table(name, std::move(schema)))).first->second;
        }

        size_t size() const {
            return tables_.size();
        }

    };

}


#endif //MOKV_TABLE_TCC
