#include <cstddef>
#include <cassert>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <iostream>

static constexpr double unset = 2.0;

double getP(std::vector<std::vector<double>>& probs, std::size_t i, std::size_t j) {
    assert(!probs.empty());
    const std::size_t n = probs.size() - 1;
    double& ret = probs[i][j];
    if (ret != unset) {
        return ret;
    }
    if (i == n && j < n) {
        ret = 1.0;
        return ret;
    }
    if (i < n && j == n) {
        ret = 0.0;
        return ret;
    }
    ret = (getP(probs, i + 1, j) + getP(probs, i, j + 1)) / 2.0;
    return ret;
}

int main() {
    constexpr std::size_t n = 10;
    std::vector<std::vector<double>> probs(n + 1, std::vector<double>(n + 1, unset));
    std::cout << getP(probs, n - 1, n - 3);


}