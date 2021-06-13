#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

std::size_t LCS(const std::string& x, const std::string& y) {
    const std::size_t m = x.length();
    const std::size_t n = y.length();
    std::vector<std::vector<std::size_t>> LCSuff(m + 1, std::vector<std::size_t>(n + 1));
    std::size_t res = 0;
    for (std::size_t i = 0; i <= m; ++i) {
        for (std::size_t j = 0; j <= n; ++j) {
            if (!i || !j) {
                LCSuff[i][j] = 0;
            } else if (x[i - 1] == y[j - 1]) {
                LCSuff[i][j] = LCSuff[i - 1][j - 1] + 1;
                res = std::max(res, LCSuff[i][j]);
            } else {
                LCSuff[i][j] = 0;
            }
        }
    }
    return res;
}

int main() {
    std::cout << LCS("leetcode", "codeleet");


}