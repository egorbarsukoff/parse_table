//
// Created by egor on 12/17/19.
//

#ifndef PARSE_TABLE__TABLE_H
#define PARSE_TABLE__TABLE_H


#include <cassert>
#include <vector>
#include <ostream>
#include <regex>
#include <numeric>
#include <iomanip>

struct Table {
    std::vector<std::string> header;
    std::vector<std::vector<double>> rows;
    [[nodiscard]] size_t ncol() const;
    [[nodiscard]] size_t nrow() const;
    explicit Table(const std::string &header_row_str);
    Table() = default;
    void add_row(const std::string& row_str);
    static Table from_stream(std::istream& is);
    friend std::istream& operator>>(std::istream & is, Table & table);
    friend std::ostream &operator<<(std::ostream &os, const Table &table);
private:
};

#endif //PARSE_TABLE__TABLE_H
