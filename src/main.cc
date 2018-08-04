#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <memory>
#include <vector>
#include <algorithm>
#include <bitset>

class Node {
public:

    static constexpr char root = -1;

    Node(
        std::unique_ptr<Node> left = nullptr,
        std::unique_ptr<Node> right = nullptr,
        std::uint64_t freq = 0,
        char c = root):
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

void createMapEncoding(std::array<char, 128> & ar, Node const & cn, int cb) {
    if (cn.isLeaf()) {
        ar[cn.getChar()] = cb;
        return;
    }
    createMapEncoding(ar, *cn.getLeftChild(), cb << 1);
    createMapEncoding(ar, *cn.getRightChild(), (cb << 1) | 0b1);
}

std::array<char, 128> createHuffmanEncoding(Node const & root) {
    std::array<char, 128> ar;
    std::fill(ar.begin(), ar.end(), -1);
    createMapEncoding(ar, root, 0b0);
    return ar;
}

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
        if (content.empty()) {
            continue;
        }
        std::array<std::uint64_t, 128> lookUpTable = {0};
        for (auto c: content) {
            ++lookUpTable[c];
        }
        std::vector<std::unique_ptr<Node>> minHeap;
        minHeap.reserve(lookUpTable.size());
        auto cmp = [](auto const & a, auto const & b) bool { return a->getFrequency() > b->getFrequency(); };
        for (auto c = 0; c < lookUpTable.size(); ++c) {
            if (lookUpTable[c] > 0) {
                minHeap.push_back(std::make_unique<Node>(nullptr, nullptr, lookUpTable[c], static_cast<char>(c)));
                std::push_heap(minHeap.begin(), minHeap.end(), cmp);
            }
        }
        while (minHeap.size() > 1) {
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
                    left->getFrequency() + right->getFrequency()
                ));
            std::push_heap(minHeap.begin(), minHeap.end(), cmp);
        }
        auto root = std::move(minHeap.front());
        minHeap.pop_back();
        auto encoding = createHuffmanEncoding(*root);
        for (auto c = 0; c < encoding.size(); ++c) {
            if (encoding[c] != -1) {
                std::cout << static_cast<char>(c) << ": hz=" << lookUpTable[c] << ", encoding="<< std::bitset<8>(encoding[c]) << std::endl;
            }
        }
    }
    return 0;
}