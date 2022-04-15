#include "util.tcc"

namespace MOKV {

std::string generateFileName(
    const std::string& db_name, const std::string& col_name,
    size_t id, size_t level)
{
    std::string s("./");
    s.reserve(db_name.size() + col_name.size() + 20);
    return s + db_name + "/" + col_name + "_" + std::to_string(id) + "_" + std::to_string(level) + ".sst";
}

}