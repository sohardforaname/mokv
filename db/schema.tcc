//
// Created by ucchi_mchxyz on 2022/1/15.
//

#ifndef MOKV_SCHEMA_TCC
#define MOKV_SCHEMA_TCC

#include <string>
#include <utility>
#include <vector>

namespace MOKV {

class Schema {
private:
    std::vector<size_t> col_len_;
    std::vector<std::string> col_name_;
    size_t len_;
    size_t primary_idx_;

public:
    Schema() = delete;

    Schema(std::vector<size_t> col_len, std::vector<std::string> col_name, size_t primary_idx)
        : col_len_(std::move(col_len))
        , col_name_(std::move(col_name))
        , len_(std::min(col_len_.size(), col_name_.size()))
        , primary_idx_(primary_idx)
    {
    }

    Schema(const Schema&) = default;

    Schema(Schema&&) = default;

    std::pair<size_t, std::string_view> getColById(size_t idx) const { return { col_len_[idx], col_name_[idx] }; }

    const std::vector<size_t>& getColLen() const { return col_len_; }

    const std::vector<std::string>& getColName() const { return col_name_; }

    size_t getPrimaryIdx() const { return primary_idx_; }

    size_t size() const { return len_; }

    ssize_t getColIdxByName(const std::string& col_name)
    {
        for (int i = 0; i < col_name_.size(); ++i) {
            if (col_name == col_name_[i]) {
                return i;
            }
        }
        return -1;
    }

    std::string toString()
    {
        std::string s(std::to_string(col_name_.size()) + '\n');
        for (int i = 0; i < len_; ++i) {
            s = s + std::to_string(col_name_[i].size()) + " "
                + col_name_[i] + " "
                + std::to_string(col_len_[i]) + '\n';
        }
        return s;
    }
};

const Schema default_schema {
    { 0, 0 },
    { "key", "value" },
    0
};

}

#endif // MOKV_SCHEMA_TCC
