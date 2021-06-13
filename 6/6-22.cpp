#include <cstddef>
#include <cassert>
#include <vector>
#include <algorithm>

bool coinChange(const std::vector<std::size_t>& d, std::size_t v) {
    const std::size_t n = d.size();
    std::vector<std::vector<int>> K(n + 1, std::vector<int>(v + 1));
    K[0][0] = 1;
    for (std::size_t i = 1; i <= n; ++i) {
        K[i][0] = 1;
        for (std::size_t j = 1; j <= v; ++j) {
            if (j < d[i - 1]) {
                K[i][j] = K[i - 1][j];
            } else {
                K[i][j] = K[i - 1][j] || K[i - 1][j - d[i - 1]];
            }
        }
    }
    return K[n][v];
}

int main() {
    assert(!coinChange(std::vector<std::size_t>({1, 2, 5}), 11));
    assert(!coinChange(std::vector<std::size_t>({2}), 3));
    assert(coinChange(std::vector<std::size_t>({1}), 0));
    assert(coinChange(std::vector<std::size_t>({1}), 1));
    assert(!coinChange(std::vector<std::size_t>({1}), 2));

}