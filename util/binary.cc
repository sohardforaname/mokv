#include "binary.tcc"

namespace MOKV {
const char* loadVariableLengthString(int fd, size_t& len)
{
    size_t slen;
    readFromBinaryFile(fd, slen);
    len = slen;
    char* buffer = new char[slen];
    readFromBinaryFile(fd, std::make_pair(buffer, slen));
    return buffer;
}
}