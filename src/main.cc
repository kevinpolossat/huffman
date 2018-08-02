#include <iostream>
#include <fstream>
#include <string>
#include <array>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << *argv << " file1 file2 ..." << std::endl;
        return 0;
    }
    for (int i = 1; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
        std::ifstream ifs(argv[i]);
        if (!ifs.is_open()) {
            std::cerr << "fail to open file " << argv[i] << std::endl;
            continue;
        }
        std::string content(
            std::istreambuf_iterator<char>(ifs),
            std::istreambuf_iterator<char>());
        std::array<int, 256> lookUpTable;
        for (auto c: content) {
            ++lookUpTable[c];
        }
    }
    return 0;
}