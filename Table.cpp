//
// Created by egor on 12/17/19.
//



#include "Table.h"

using namespace std::string_literals;

std::ostream &operator<<(std::ostream &os, const Table &table) {
    std::vector<std::vector<std::string>> string_repr;
    string_repr.reserve(table.nrow() + 1);
    string_repr.push_back(table.header);
    for (auto &row : table.rows) {
        string_repr.emplace_back();
        string_repr.reserve(table.ncol());
        for (auto el : row) {
            string_repr.back().push_back(std::to_string(el));
        }
    }
    std::vector<size_t> colw;
    colw.reserve(table.ncol());
    for (size_t col = 0; col < string_repr.back().size(); ++col) {
        size_t max = 0;
        for (auto &row : string_repr) {
            max = std::max(max, row[col].size() + 2);
        }
        colw.push_back(max);
    }
    for (auto &row : string_repr) {
        for (size_t i = 0; i < row.size(); ++i) {
            os << std::setw(colw[i]) << row[i];
        }
        os << std::endl;
    }
    return os;
}

Table::Table(const std::string &header_row_str) {
    static std::regex spaces_re{R"(\s+)"};
    auto start_iter = std::sregex_token_iterator(header_row_str.cbegin(), header_row_str.cend(), spaces_re, -1);
    auto stop_iter = std::sregex_token_iterator{};
    header.reserve(std::distance(start_iter, stop_iter));
    std::transform(start_iter, stop_iter, std::back_inserter(header), [] (auto& match) {
        return match.str();
    });
}

void Table::add_row(const std::string &row_str) {
    static std::regex number{R"([+-]?(\d+([.]\d*)?(e[+-]?\d+)?|[.]\d+(e[+-]?\d+)?))", std::regex::icase};
    static std::regex spaces_re{R"(\s+)"};
    auto start_iter = std::sregex_token_iterator(row_str.cbegin(), row_str.cend(), spaces_re, -1);
    auto stop_iter = std::sregex_token_iterator{};
    auto len = std::distance(start_iter, stop_iter);
    if (len != ncol()) {
        throw std::runtime_error{"Wrong number of columns in row \n"s + row_str};
    }
    rows.emplace_back();
    rows.back().reserve(len);
    std::transform(start_iter, stop_iter, std::back_inserter(rows.back()), [] (auto& match) {
        std::string s = match.str();
        if (!std::regex_match(s, number)) {
            throw std::runtime_error("Cannot parse "s + s);
        }
        return std::stod(s);
    });
}

size_t Table::ncol() const {
    return header.size();
}

size_t Table::nrow() const {
    return rows.size();
}
std::istream &operator>>(std::istream &is, Table &table) {
    std::string s;
    std::getline(is, s);
    table = Table{s};

    while (is) {
        std::getline(is, s);
        if (s.empty()) {
            break;
        }

        table.add_row(s);

    }
    return is;
}

Table Table::from_stream(std::istream &is) {
    Table t;
    is >> t;
    return t;
}
