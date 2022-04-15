//
// Created by ucchi_mchxyz on 2022/1/13.
//

#include "table.tcc"

using namespace MOKV;

void test()
{
    TableSet set;
    auto t = set.open("test_db");
    if (!t) {
        t = set.create("test_db", default_schema);
    }

    t->write("key1", "value1");
    t->write("key2", "value3");

    MOKV::Batch batch;
    batch.put("key3", "value5");
    batch.put("key5", "value10");
    batch.put("key4", "value3");

    t->writeBatch(std::move(batch));
}

int main()
{
    test();
    return 0;
}