#include <algorithm>
#include <cassert>
#include <bitset>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <utility>
#include <tuple>
#include <list>

std::size_t getMaxIndex(const std::vector<std::tuple<int, int, int>>& threeSAT) {
    int n = 0;
    for (const auto& [i, j, k] : threeSAT) {
        n = std::max({n, std::abs(i), std::abs(j), std::abs(k)});
    }
    return n;
}

bool isSatisfiable(const std::vector<std::tuple<int, int, int>>& threeSAT) {
    if (threeSAT.empty()) {
        return true;
    }
    auto n = getMaxIndex(threeSAT);
    std::unordered_set<std::bitset<10>> bitsets;
    for (std::size_t i = 0; i < 1024u; ++i) {
        bitsets.insert(std::bitset<10>(i));
    }
    for (int i = 1; i <= n; ++i) {
        // window from i ~ i + 9
        for (const auto& [a, b, c] : threeSAT) {
            auto a_ = std::abs(a);
            auto b_ = std::abs(b);
            auto c_ = std::abs(c);
            if (a_ < i || a_ > i + 9 || b_ < i || b_ > i + 9 || c_ < i || c_ > i + 9) {
                continue;
            }
            std::unordered_set<std::bitset<10>> new_bitsets;
            for (const auto& bset : bitsets) {
                bool satA = (a > 0) == bset[a_ - i];
                bool satB = (b > 0) == bset[b_ - i];
                bool satC = (c > 0) == bset[c_ - i];
                if (satA || satB || satC) {
                    new_bitsets.insert(bset);
                }
            }
            std::swap(new_bitsets, bitsets);
            if (bitsets.empty()) {
                return false;
            }
        }
        std::unordered_set<std::bitset<10>> new_bitsets;
        for (auto& bset : bitsets) {
            new_bitsets.insert(bset << 1u);
        }
        std::swap(new_bitsets, bitsets);
    }
    return !bitsets.empty();
}

int main() {

}
