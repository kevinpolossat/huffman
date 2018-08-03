#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <memory>
#include <vector>
#include <algorithm>

class Node {
public:
    Node(
        std::unique_ptr<Node> left = nullptr,
        std::unique_ptr<Node> right = nullptr,
        std::uint64_t freq = 0,
        char c = 0):
        left_(std::move(left)),
        right_(std::move(right)),
        freq_(freq),
        c_(c)
        {}

        char getChar() const { return c_; }
        std::uint64_t getFrequency() const { return freq_; }
        std::unique_ptr<Node> const & getLeftChild() const { return left_; }
        std::unique_ptr<Node> const & getRightChild() const { return right_; }
        bool isLeaf() const { return !left_ && !right_; } 
private:
    std::unique_ptr<Node> left_;
    std::unique_ptr<Node> right_;
    std::uint64_t freq_;
    char c_;
};

std::ostream & operator<<(std::ostream & os, Node const & n) {
    if (n.isLeaf()) {
        os << "[" << n.getChar() << ", " << n.getFrequency() << "], "; 
    } else {
        os << "[internal, " << n.getFrequency() << "], "; 
        os << *n.getLeftChild();
        os << *n.getRightChild();
    }
    return os;
}

static constexpr char root = -1;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << *argv << " file1 file2 ..." << std::endl;
        return 0;
    }
    for (auto i = 1; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
        std::ifstream ifs(argv[i]);
        if (!ifs.is_open()) {
            std::cerr << "fail to open file " << argv[i] << std::endl;
            continue;
        }
        std::string content{
            std::istreambuf_iterator<char>(ifs),
            std::istreambuf_iterator<char>()};
        std::array<std::uint64_t, 128> lookUpTable = {0};
        for (auto c: content) {
            ++lookUpTable[c];
        }
        std::vector<std::unique_ptr<Node>> minHeap;
        minHeap.reserve(lookUpTable.size());
        auto cmp = [&lookUpTable](auto const & a, auto const & b) bool { return a->getFrequency() > b->getFrequency(); };
        for (auto c = 0; c < lookUpTable.size(); ++c) {
            if (lookUpTable[c] > 0) {
                std::cout << static_cast<char>(c) << " " << lookUpTable[c] << std::endl;
                minHeap.push_back(std::make_unique<Node>(nullptr, nullptr, lookUpTable[c], static_cast<char>(c)));
                std::push_heap(minHeap.begin(), minHeap.end(), cmp);
            }
        }
        while (minHeap.size() > 1) {
            std::make_heap(minHeap.begin(), minHeap.end(), cmp);
            std::pop_heap(minHeap.begin(), minHeap.end(), cmp);
            auto left = std::move(minHeap.back());
            minHeap.pop_back();
            std::pop_heap(minHeap.begin(), minHeap.end(), cmp);
            auto right = std::move(minHeap.back());
            minHeap.pop_back();
            minHeap.push_back(
                std::make_unique<Node>(
                    std::move(left),
                    std::move(right),
                    left->getFrequency() + right->getFrequency(),
                    root));
            std::push_heap(minHeap.begin(), minHeap.end(), cmp);
        }
        std::cout << "res=" << *minHeap.front() << std::endl;
    }
    return 0;
}