#include <cstddef>
#include <cassert>
#include <vector>
#include <algorithm>

bool coinChange(const std::vector<std::size_t>& d, std::size_t v) {
    std::vector<int> DP(v + 1);
    DP[0] = 1;
    for (std::size_t i = 1; i <= v; ++i) {
        for (auto denom : d) {
            if (denom <= i && DP[i - denom]) {
                DP[i] = true;
                break;
            }
        }
    }
    return DP[v];
}

int main() {
    assert(coinChange(std::vector<std::size_t>({1, 2, 5}), 11));
    assert(!coinChange(std::vector<std::size_t>({2}), 3));
    assert(coinChange(std::vector<std::size_t>({1}), 0));
    assert(coinChange(std::vector<std::size_t>({1}), 1));
    assert(coinChange(std::vector<std::size_t>({1}), 2));

}