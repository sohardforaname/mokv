//
// Created by ucchi_mchxyz on 2021/12/1.
//

#ifndef MOKV_BINARY_TCC
#define MOKV_BINARY_TCC

#include <algorithm>
#include <cstring>
#include <unistd.h>

namespace MOKV {
template <class T, class Ty, class Size>
void read(const T* buffer, std::pair<Ty*, Size>& v, size_t& cur_pos)
{
    memcpy(v.first, buffer, sizeof(Ty) * v.second);
    cur_pos += sizeof(Ty) * v.second;
}

template <class T, class Ty>
void read(const T* buffer, Ty& v, size_t& cur_pos)
{
    memcpy(&v, buffer, sizeof(v));
    cur_pos += sizeof(v);
}

template <class T, class... Ts>
size_t readFromBuffer(const T* buffer, Ts&&... v)
{
    size_t cur_pos = 0;
    (read(buffer + cur_pos, v, cur_pos), ...);
    return cur_pos;
}

template <class T, class Ty, class Size>
void write(T* buffer, std::pair<Ty*, Size>& v, size_t& cur_pos)
{
    memcpy(buffer, v.first, sizeof(Ty) * v.second);
    cur_pos += sizeof(Ty) * v.second;
}

template <class T, class Ty>
void write(T* buffer, Ty& v, size_t& cur_pos)
{
    memcpy(buffer, &v, sizeof(v));
    cur_pos += sizeof(v);
}

template <class T, class... Ts>
size_t writeToBuffer(T* buffer, Ts&&... v)
{
    size_t cur_pos = 0;
    (write(buffer + cur_pos, v, cur_pos), ...);
    return cur_pos;
}

template <class T, class Size>
void read(int fd, std::pair<T*, Size>& sized_ptr)
{
    ::read(fd, sized_ptr.first, sizeof(T) * sized_ptr.second);
}

template <class T>
void read(int fd, T& v)
{
    ::read(fd, &v, sizeof(T) * 1);
}

template <class... Ts>
void readFromBinaryFile(int fd, Ts&&... v1)
{
    (read(fd, v1), ...);
}

template <class T, class Size>
void write(int fd, std::pair<T*, Size>& sized_ptr)
{
    ::write(fd, sized_ptr.first, sizeof(T) * sized_ptr.second);
}

template <class T>
void write(int fd, T& v)
{
    ::write(fd, &v, sizeof(T) * 1);
}

template <class... Ts>
void writeToBinaryFile(int fd, Ts&&... v1)
{
    (write(fd, v1), ...);
}

const char* loadVariableLengthString(int fd, size_t& len);
}

#endif // MOKV_BINARY_TCC