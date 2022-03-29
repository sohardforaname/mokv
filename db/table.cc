//
// Created by ucchi_mchxyz on 2022/1/13.
//

#include "table.tcc"

namespace DB {

    void Table::write(const char *key, const char *value) {
        Batch batch;
        batch.put(key, value);
        writeBatch(std::move(batch));
    }

    void Table::writeBatch(Batch batch) {
        for (auto &p : batch.dat()) {
            mem_->insert(p.first, p.second, ValueType::Value);
        }

        imm_.emplace_back(mem_);

        std::thread dump_thread([&]() {
            imm_.back()->generateSSTableFile("test_db", default_schema);
        });
        // dump_thread.detach();
        dump_thread.join();

        mem_ = new MemTable;
    }

    const char *Table::find(const char *key, size_t col_idx) {

        auto f = [&](MemTable *mem) -> const char * {
            MemTable::ref(mem);
            auto ptr = mem->find(key);
            MemTable::unref(mem);
            return ptr;
        };

        if (const auto ptr = f(mem_); nullptr != ptr) {
            return ptr;
        }
        for (auto p : imm_) {
            if (const auto ptr = f(p); nullptr != ptr) {
                return ptr;
            }
        }

        auto col_name = schema_.getColName()[col_idx];
        auto fd = pool_.openFile(("./" + name_ + "/" + col_name + ".bin").c_str());
        if (-1 == fd) {
            return nullptr;
        }
        auto page_id = pool_.getPage(fd, 0);
        if (-1 == fd) {
            return nullptr;
        }
        auto raw_ptr = pool_.getData(page_id);
    }

    RangeIterator Table::range() {
        return RangeIterator();
    }
}