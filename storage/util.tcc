#include <string>

#ifndef MOKV_UTIL_TCC
#define MOKV_UTIL_TCC

namespace MOKV {

std::string generateFileName(
    const std::string& db_name, const std::string& col_name,
    size_t id, size_t level);

}

#endif