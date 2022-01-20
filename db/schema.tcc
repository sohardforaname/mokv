//
// Created by ucchi_mchxyz on 2022/1/15.
//

#ifndef MOKV_SCHEMA_TCC
#define MOKV_SCHEMA_TCC

#include <utility>
#include <vector>
#include <string>

namespace DB {

    class Schema {
    private:
        std::vector<size_t> col_len_;
        std::vector<std::string> col_name_;
        size_t len_;

    public:
        Schema() = delete;

        Schema(std::vector<size_t> col_len, std::vector<std::string> col_name)
                : col_len_(std::move(col_len)),
                  col_name_(std::move(col_name)),
                  len_(std::min(col_len_.size(), col_name_.size())) {}

        Schema(const Schema &) = default;

        Schema(Schema &&) = default;

        std::pair<size_t, std::string_view> getColById(size_t idx) const {
            return {col_len_[idx], col_name_[idx]};
        }

        const std::vector<size_t> &getColLen() const {
            return col_len_;
        }

        const std::vector<std::string> &getColName() const {
            return col_name_;
        }

        size_t size() const {
            return len_;
        }
    };

    const Schema default_schema{
            {0,     0},
            {"key", "value"}
    };

}

#endif //MOKV_SCHEMA_TCC
