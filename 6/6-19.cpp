#include <cstddef>
#include <cassert>
#include <vector>
#include <algorithm>

bool coinChange(const std::vector<std::size_t>& d, std::size_t v, std::size_t k) {
    std::vector<int> DP(v + 1, static_cast<int>(v + 1));
    DP[0] = 0;
    for (std::size_t i = 1; i <= v; ++i) {
        for (auto denom : d) {
            if (denom <= i) {
                DP[i] = std::min(DP[i], DP[i - denom] + 1);
            }
        }
    }
    return DP[v] <= v && DP[v] <= k;
}

int main() {
    assert(coinChange(std::vector<std::size_t>({1, 2, 5}), 11, 3));
    assert(!coinChange(std::vector<std::size_t>({2}), 3, 3));
    assert(coinChange(std::vector<std::size_t>({1}), 0, 3));
    assert(coinChange(std::vector<std::size_t>({1}), 1, 3));
    assert(!coinChange(std::vector<std::size_t>({1}), 2, 1));

}