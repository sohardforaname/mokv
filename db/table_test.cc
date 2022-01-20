//
// Created by ucchi_mchxyz on 2022/1/13.
//

#include "table.tcc"

using namespace DB;

void test() {
    TableSet set;
    auto t = set.create("test", default_schema);

    t->write("key1", "value1");
}

int main() {
    test();
    return 0;
}