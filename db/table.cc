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
        auto key_size = strlen(p.first), value_size = strlen(p.second);

        char* key_buf = new char[key_size + sizeof(size_t)];
        strcpy(key_buf, p.first);
        writeToBuffer(key_buf + key_size, line_id_);
        key_size += sizeof(size_t);

        char* value_buf = new char[value_size + sizeof(size_t)];
        strcpy(value_buf, p.second);
        writeToBuffer(value_buf + value_size, line_id_);
        value_size += sizeof(size_t);

        ++line_id_;
        mem_->insert(key_buf, key_size, value_buf, value_size, ValueType::Value);
    }

    imm_.emplace_back(mem_);

    std::thread dump_thread([&]() {
        imm_.back()->generateSSTableFile((this->name_).c_str(), default_schema, l0_sst_id_++);
        // std::lock_guard lock(mutex_);
    });
    // dump_thread.detach();
    dump_thread.join();

    mem_ = new MemTable;
}

const char* Table::find(const char* key, const std::string& col_name)
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

    // 打开这一列的所有层数的文件，然后用迭代器查找
    // 文件名是 ./{table_name}/{col_name}_{id}_{level}.sst
    // 主键列
    int idx = schema_.getColIdxByName(col_name);
    if (idx != schema_.getPrimaryIdx()) {
    }
    for (int i = 0; i < SSTABLE_MAX_LEVEL; ++i) {
        for (int j = l0_sst_id_; j >= 0; --j) {
            int fd = files_.openFile(generateFileName(name_, col_name, j, i).c_str());
            if (-1 == fd) {
                continue;
            }
        }
    }

    return nullptr;
}

int Table::close()
{
    if (-1 == lseek(schema_fd_, 0, SEEK_SET)) {
        return errno;
    }
    auto s = schema_.toString()
        + std::to_string(l0_sst_id_) + "\n"
        + std::to_string(line_id_) + "\n";

    if (-1 == ::write(schema_fd_, s.c_str(), s.size())) {
        return errno;
    }
    ::close(schema_fd_);
    return 0;
}

Schema getSchema(FILE* fp)
{
    size_t col_size;
    fscanf(fp, "%zu", &col_size);
    std::vector<std::string> col_name;
    std::vector<size_t> col_len;
    while (col_size > 0) {
        size_t l, col_l;
        fscanf(fp, "%zu", &l);
        char* buf = new char[l + 1];
        fscanf(fp, "%s%zu", buf, &col_l);
        col_name.push_back(buf);
        col_len.push_back(col_l);
    }
    return Schema(std::move(col_len), std::move(col_name), 0);
}

Table* TableSet::open(std::string db_name)
{
    auto path = std::move("./" + db_name);
    if (-1 == access(path.c_str(), F_OK)) {
        return nullptr;
    }
    auto schema_file = path + "/" + db_name + ".sma";
    if (-1 == access(schema_file.c_str(), F_OK)) {
        return nullptr;
    }
    auto fd = ::open(schema_file.c_str(), O_RDWR, 0666);
    if (-1 == fd) {
        return nullptr;
    }
    FILE* fp = fdopen(fd, "r+");
    if (!fp) {
        return nullptr;
    }

    Schema schema(getSchema(fp));
    size_t sst_id, line_id;
    fscanf(fp, "%zu%zu", &sst_id, &line_id);

    return &tables_.try_emplace(
                       std::move(db_name),
                       std::move(Table(db_name, std::move(schema), fd,
                           sst_id, line_id)))
                .first->second;
}

Table* TableSet::create(std::string db_name, Schema schema)
{

    auto path = std::move("./" + db_name);
    umask(0);
    if (-1 == mkdir(path.c_str(), 0777)) {
        return nullptr;
    }
    auto schema_file = path + "/" + db_name + ".sma";

    auto fd = ::open(schema_file.c_str(), O_CREAT | O_WRONLY, 0666);
    if (-1 == fd) {
        return nullptr;
    }
    auto schema_string = schema.toString();
    if (-1 == ::write(fd, schema_string.c_str(), schema_string.size())) {
        return nullptr;
    }
    std::string table_info("0\n0\n");
    if (-1 == ::write(fd, table_info.c_str(), table_info.size())) {
        return nullptr;
    };

    return &tables_.try_emplace(std::move(db_name), std::move(Table(db_name, std::move(schema), fd))).first->second;
}

RangeIterator Table::range()
{
    return RangeIterator();
}
}