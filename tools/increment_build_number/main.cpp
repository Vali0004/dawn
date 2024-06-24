#include <iostream>
#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

inline std::string read_line(std::filesystem::path path, std::string search) {
    std::ifstream ifile{ path };
    std::string line{};

    while (std::getline(ifile, line)) {
        if (line.find(search) != std::string::npos || !line.compare(search)) {
            return line;
        }
    }
    
    return {};
}

inline void overwrite_line(std::filesystem::path path, std::string search, std::string value) {
    std::ifstream ifile{ path };
    std::vector<std::string> lines{};
    std::string line{};

    while (std::getline(ifile, line)) {
        if (line.find(search) != std::string::npos || !line.compare(search)) {
            line = value;
        }

        lines.push_back(line);
    }

    ifile.close();

    std::ofstream ofile{ path };

    for (auto& l : lines) {
        ofile << l << std::endl;
    }

    ofile.close();
}

std::filesystem::path interpret_path(std::string path, std::string relative_path) {
    try {
        std::filesystem::path base_path{ path };
        std::filesystem::path rel_path{ relative_path };
        return std::filesystem::canonical(base_path / rel_path);
    }
    catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }
    return {};
}

int main(int argc, char** argv) {
    std::filesystem::path file_path{ interpret_path(argv[0], "../../../../src/build_number.h") };
    if (!std::filesystem::exists(file_path)) {
        std::cerr << "Error getting canonical path or path does not exist." << std::endl;
        return 1;
    }

    std::ifstream file_in{ file_path.string() };
    if (!file_in) {
        std::ofstream file_out{ file_path.string() };
        file_out << "#pragma once\n";
        file_out << "#define BUILD \"b0001\"\n";
        file_out << "\n";
    }
    file_in.close();

    int build_num{};

    std::string search{ "#define BUILD \"b" };
    std::string replacement{ search };
    std::string str{ read_line(file_path, search) };

    if (str.empty()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return 1;
    }

    std::string build_num_str{ str.substr(search.length()) };
    build_num = std::stoi(build_num_str);

    std::cout << "Previous build number: " << build_num << std::endl;
    build_num += 1;
    replacement += std::to_string(build_num) + "\"";
    std::cout << "Current build number: " << build_num << std::endl;
    overwrite_line(file_path, search, replacement);

    return 0;
}