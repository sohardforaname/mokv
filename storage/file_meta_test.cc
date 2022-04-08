#include "file_meta.tcc"
#include <cstdio>
#include <string>

void test()
{
    MOKV::FileMeta meta;
    if (meta.openFile("./test_db/key_2_0.sst")) {
        exit(errno);
    }
    auto [x, y] = meta.getMinMax();
    printf("%d\n", errno);
    printf("%s %s\n", std::string(x).c_str(), std::string(y).c_str());
    auto sz = meta.kvSize();
    printf("%zu\n", sz);
    auto buffer = meta.getBuffer();
    printf("%d\n", errno);
}

int main()
{
    test();
    return 0;
}