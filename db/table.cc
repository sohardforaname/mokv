//
// Created by ucchi_mchxyz on 2022/1/13.
//

#include "table.tcc"

namespace MOKV {

void Table::write(const char* key, const char* value)
{
    Batch batch;
    batch.put(key, value);
    writeBatch(std::move(batch));
}

void Table::writeBatch(Batch batch)
{
    for (auto& p : batch.dat()) {
        mem_->insert(p.first, p.second, ValueType::Value);
    }

    imm_.emplace_back(mem_);

    std::thread dump_thread([&]() {
        imm_.back()->generateSSTableFile((this->name_).c_str(), default_schema, l0_sst_id_++);
    });
    // dump_thread.detach();
    dump_thread.join();

    mem_ = new MemTable;
}

const char* Table::find(const char* key, size_t col_idx)
{

    auto f = [&](MemTable* mem) -> const char* {
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

    for (int i = 0; i < SSTABLE_MAX_LEVEL; ++i) {
        // for (auto& f : files_.getFilesOnLevel(i)) {
        // }
    }

    return nullptr;
}

RangeIterator Table::range()
{
    return RangeIterator();
}
}