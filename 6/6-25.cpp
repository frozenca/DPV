#include <cassert>
#include <cstddef>
#include <vector>
#include <numeric>

bool threePartition(const std::vector<std::size_t>& A) {
    auto s = std::accumulate(A.begin(), A.end(), 0lu);
    if (s % 3) {
        return false;
    }
    const std::size_t n = A.size();
    std::vector<std::vector<int>> DP(s + 1, std::vector<int>(s + 1));
    DP[0][0] = true;

    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = s; j <= s; --j) {
            for (std::size_t k = s; k <= s; --k) {
                if (DP[j][k]) {
                    DP[j + A[i]][j] = true;
                    DP[j][k + A[i]] = true;
                }
            }
        }
    }
    return DP[s / 3][s / 3];
}

int main() {
    assert(threePartition(std::vector<std::size_t>({1, 2, 3, 4, 4, 5, 8})));
    assert(!threePartition(std::vector<std::size_t>({2, 2, 3, 5})));

}