//
// Created by egor on 12/17/19.
//



#include "Table.h"

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
    static std::regex ts_names{R"(\w+(\s\w+)*\s*)"};

    if (!std::regex_match(header_row_str, ts_names)) {
        throw std::runtime_error{"Header row is not tab (or space) separated"};
    }

    static std::regex words{R"(\w+)"};
    auto start_iter = std::sregex_token_iterator(header_row_str.cbegin(), header_row_str.cend(), words);
    auto stop_iter = std::sregex_token_iterator{};
    header.reserve(std::distance(start_iter, stop_iter));
    for (auto it = start_iter; it != stop_iter; ++it) {
        header.push_back(*it);
    }
}

void Table::add_row(const std::string &row_str) {
    static std::regex number{R"([+-]?(\d+(\.\d*)?)|(\.\d*))"};
    auto start_iter = std::sregex_token_iterator(row_str.cbegin(), row_str.cend(), number);
    auto stop_iter = std::sregex_token_iterator{};
    auto len = std::distance(start_iter, stop_iter);
    if (len != ncol()) {
        throw std::runtime_error{"Failed to parse row \n" + row_str};
    }
    rows.emplace_back();
    rows.back().reserve(len);
    for (auto it = start_iter; it != stop_iter; ++it) {
        double t;
        std::stringstream{*it} >> t;
        rows.back().push_back(t);
    }

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
