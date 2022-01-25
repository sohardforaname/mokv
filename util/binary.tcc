//
// Created by ucchi_mchxyz on 2021/12/1.
//

#ifndef MOKV_BINARY_TCC
#define MOKV_BINARY_TCC

#include <cstdio>
#include <algorithm>
#include <cstring>

namespace DB {
    template<class T, class Ty, class Size>
    void read(const T *buffer, std::pair<Ty *, Size> &v, size_t &cur_pos) {
        memcpy(v.first, buffer, sizeof(Ty) * v.second);
        cur_pos += sizeof(Ty) * v.second;
    }

    template<class T, class Ty>
    void read(const T *buffer, Ty &v, size_t &cur_pos) {
        memcpy(&v, buffer, sizeof(v));
        cur_pos += sizeof(v);
    }

    template<class T, class ...Ts>
    size_t readFromBuffer(const T *buffer, Ts &&...v) {
        size_t cur_pos = 0;
        (read(buffer + cur_pos, v, cur_pos), ...);
        return cur_pos;
    }

    template<class T, class Ty, class Size>
    void write(T *buffer, std::pair<Ty *, Size> &v, size_t &cur_pos) {
        memcpy(buffer, v.first, sizeof(Ty) * v.second);
        cur_pos += sizeof(Ty) * v.second;
    }

    template<class T, class Ty>
    void write(T *buffer, Ty &v, size_t &cur_pos) {
        memcpy(buffer, &v, sizeof(v));
        cur_pos += sizeof(v);
    }

    template<class T, class ...Ts>
    size_t writeToBuffer(T *buffer, Ts &&...v) {
        size_t cur_pos = 0;
        (write(buffer + cur_pos, v, cur_pos), ...);
        return cur_pos;
    }

    template<class T, class Size>
    void read(FILE *file, std::pair<T *, Size> &sized_ptr) {
        fread(sized_ptr.first, sizeof(T), sized_ptr.second, file);
    }

    template<class T>
    void read(FILE *file, T &v) {
        fread(&v, sizeof(T), 1, file);
    }

    template<class ...Ts>
    void readFromBinaryFile(FILE *file, Ts &&... v1) {
        (read(file, v1), ...);
    }

    template<class T, class Size>
    void write(FILE *file, std::pair<T *, Size> &sized_ptr) {
        fwrite(sized_ptr.first, sizeof(T), sized_ptr.second, file);
    }

    template<class T>
    void write(FILE *file, T &v) {
        fwrite(&v, sizeof(T), 1, file);
    }

    template<class ...Ts>
    void writeToBinaryFile(FILE *file, Ts &&... v1) {
        (write(file, v1), ...);
    }
}

#endif //MOKV_BINARY_TCC