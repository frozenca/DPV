#include <cstddef>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <utility>

enum class Edit {
    Add,
    Remove,
    Change
};

std::pair<std::size_t, std::vector<std::size_t>> computeK(const std::string& s1, const std::string& s2,
                     std::size_t begin1, std::size_t begin2,
                     std::size_t end1, std::size_t end2,
                     std::vector<std::size_t> E) {
    std::size_t mid2 = begin2 + (end2 - begin2) / 2;
    std::size_t k = begin1;
    std::size_t max_E_mid = E.size();
    for (std::size_t i = begin1 + 1; i <= end1; ++i) {
        std::vector<std::size_t> nextE (E.size());
        nextE[0] = i;
        for (std::size_t j = 1; j < E.size(); ++j) {
            auto val1 = E[j] + 1;
            auto val2 = nextE[j - 1] + 1;
            auto val3 = E[j - 1];
            if (s1[i - 1] != s2[j - 1]) {
                ++val3;
            }
            auto val = std::min({val1, val2, val3});
            nextE[j] = val;
            if (j == mid2 + 1 && val <= max_E_mid) {
                if (val == val2) {
                    k = i;
                } else {
                    k = i - 1;
                }
                max_E_mid = val;
            }
        }
        E = std::move(nextE);
    }
    return {k, E};
}

std::vector<Edit> getEditSequence(const std::string& s1, const std::string& s2,
                                  std::size_t begin1, std::size_t begin2,
                                  std::size_t end1, std::size_t end2,
                                  std::vector<std::size_t>& editDistanceRow) {
    if (end2 - begin2 == 0) {
        std::size_t k = end1 - begin1;
        return std::vector<Edit>(k, Edit::Remove);
    } else if (end2 - begin2 == 1) {
        std::size_t k = end1 - begin1;
        std::vector<Edit> ES;
        if (k > 1) {
            ES = std::vector<Edit>(k - 1, Edit::Remove);
        }
        k = std::max(k, 1lu);
        if (k == 0) {
            ES.push_back(Edit::Add);
        } else if (k == 1 && s1[begin1] != s2[begin2]) {
            ES.push_back(Edit::Change);
        }
        return ES;
    }
    auto [k, newEditDistanceRow] = computeK(s1, s2, begin1, begin2, end1, end2, editDistanceRow);
    auto upperSequence = getEditSequence(s1, s2, begin1,  begin2, k, begin2 + (end2 - begin2) / 2, editDistanceRow);
    auto lowerSequence = getEditSequence(s1, s2, k, begin2 + (end2 - begin2) / 2, end1, end2, editDistanceRow);
    editDistanceRow = std::move(newEditDistanceRow);
    std::vector<Edit> res;
    std::ranges::move(upperSequence, std::back_inserter(res));
    std::ranges::move(lowerSequence, std::back_inserter(res));
    return res;
}

std::vector<Edit> getEditSequence(const std::string& s1, const std::string& s2) {
    const std::size_t m = s1.size();
    const std::size_t n = s2.size();
    std::vector<std::size_t> E(n + 1);
    for (std::size_t j = 0; j <= n; ++j) {
        E[j] = j;
    }
    return getEditSequence(s1, s2, 0, 0, m, n, E);
}

int main() {
    auto edit_seq = getEditSequence("EXPONENTIAL", "POLYNOMIAL");
    for (auto edit : edit_seq) {
        std::cout << static_cast<int>(edit) << ' ';
    }



}