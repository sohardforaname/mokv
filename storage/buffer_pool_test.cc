//
// Created by ucchi_mchxyz on 2021/12/26.
//

#include <unistd.h>
#include "buffer_pool.tcc"
#include "../util/binary.tcc"

void test() {
    size_t arr[64];
    DB::BufferPool &pool = DB::BufferPool::getBufferPool();

    auto fn = pool.openFile("D:\\test.bin");
    for (int i = 0; i < 3; ++i) {
        auto pn = pool.getPage(fn, i * sizeof(size_t));

        auto ptr = pool.getData(pn);

        DB::readFromBuffer(ptr, std::make_pair(arr, 64));
        printf("{ ");
        for (auto v : arr) {
            printf("%zu ", v);
        }
        printf("}\n");
    }
}

int main() {
    if (-1 == access("D:\\test.bin", F_OK)) {
        FILE *fp = fopen("D:\\test.bin", "wb");
        if (nullptr == fp) {
            return -1;
        }
        for (size_t i = 1; i <= 4096; ++i) {
            fwrite(&i, sizeof(size_t), 1, fp);
        }
        fclose(fp);
    }

    test();
    return 0;
}