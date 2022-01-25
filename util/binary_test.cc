//
// Created by ucchi_mchxyz on 2021/12/1.
//

#include "binary.tcc"

int main() {
    auto buffer = new int[32];
    size_t a = 10;
    size_t b[5] = {0, 1, 2, 3, 4};
    DB::writeToBuffer(buffer, std::make_pair(b, 5), a);

    size_t c;
    size_t d[5];
    DB::readFromBuffer(buffer, std::make_pair(d, 5), c);

    printf("%zu\n", c);
    printf("%zu, %zu, %zu, %zu, %zu\n", d[0], d[1], d[2], d[3], d[4]);

    DB::readFromBuffer(buffer, std::make_pair(d, 5), c);

    printf("%zu\n", c);
    printf("%zu, %zu, %zu, %zu, %zu\n", d[0], d[1], d[2], d[3], d[4]);

    auto fp = fopen("D:\\test.dat", "wb+");
    if (!fp) {
        return -1;
    }

    DB::writeToBinaryFile(fp, std::make_pair(b, 5), a);

    fclose(fp);
    fp = fopen("D:\\test.dat", "rb");
    if (!fp) {
        return -1;
    }

    DB::readFromBinaryFile(fp, std::make_pair(d, 5), c);

    printf("%zu\n", c);
    printf("%zu, %zu, %zu, %zu, %zu\n", d[0], d[1], d[2], d[3], d[4]);

    return 0;
}