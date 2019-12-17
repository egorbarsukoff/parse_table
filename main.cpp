#include <iostream>
#include <fstream>
#include <vector>

#include "Table.h"

std::ifstream parse_args(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid arguments\n"
                     "Usage: parse_table file_name\n";
        std::exit(1);
    }
    std::ifstream file{argv[1]};
    if (!file.is_open()) {
        std::cerr << "Couldn't open file " << argv[1] << "\n";
        std::exit(1);
    }
    return file;
}



int main(int argc, char* argv[]) {
    auto file = parse_args(argc, argv);
    std::cout << Table::from_stream(file);
    return 0;
}
