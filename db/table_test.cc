//
// Created by ucchi_mchxyz on 2022/1/13.
//

#include "table.tcc"
#include <cassert>

using namespace MOKV;

void test_write()
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
    set.close("test_db");
}

void test_read()
{
    TableSet set;
    auto t = set.open("test_db");
    assert(t);

    std::vector<std::string> dat;
    std::vector<int> idx;
    t->findAll("value", dat, idx, [](std::string_view view) -> bool {
        return memcmp(view.data(), "value2", 6);
    });

    printf("%zu\n", dat.size());
    for (size_t i = 0; i < dat.size(); ++i) {
        printf("%s %d\n", dat[i].c_str(), idx[i]);
    }
    set.close("test_db");
}
int main()
{
    // test_write();
    test_read();
    return 0;
}