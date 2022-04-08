//
// Created by ucchi_mchxyz on 2022/1/9.
//

#ifndef MOKV_DB_TCC
#define MOKV_DB_TCC

#include "../storage/memtable.tcc"
#include "range.tcc"
#include "table.tcc"

namespace MOKV {

class DB {
private:
    TableSet opened_table_;

public:
    DB() = delete;

    static DB* create(const std::string& db_name);

    static DB* open(const std::string& db_name);

    Table* openTable(const std::string& table_name);
};

}

#endif // MOKV_DB_TCC
