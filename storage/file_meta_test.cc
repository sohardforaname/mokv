#include "file_meta.tcc"
#include <cstdio>
#include <string>

void test()
{
    DB::FileMeta meta;
    if (meta.openFile("./test_db/key_0.sst")) {
        exit(errno);
    }
    auto [x, y] = meta.getMinMax();
    printf("%s %s\n", std::string(x).c_str(), std::string(y).c_str());
    auto sz = meta.kvSize();
    printf("%zu\n", sz);
}

int main()
{
    test();
    return 0;
}