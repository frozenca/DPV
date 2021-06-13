#include <cstddef>
#include <vector>
#include <cassert>
#include <iostream>
#include <random>
#include <utility>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <algorithm>

std::unordered_map<std::size_t, char> charIndices {{'A', 0},
                                                   {'C', 1},
                                                   {'T', 2},
                                                   {'G', 3}};

std::pair<std::string, std::string> maxAlignment(const std::string& x, const std::string& y,
                                                 const std::vector<std::vector<double>>& delta,
                                                 double c_0, double c_1) {
    assert(c_0 > c_1);
    const std::size_t m = x.length();
    const std::size_t n = y.length();

    std::vector<std::vector<double>> P(m + 1, std::vector<double>(n + 1));
    std::vector<std::vector<double>> F(m + 1, std::vector<double>(n + 1));
    std::vector<std::vector<double>> G(m + 1, std::vector<double>(n + 1));
    std::vector<std::vector<double>> H(m + 1, std::vector<double>(n + 1));
    std::vector<std::vector<std::pair<std::string, std::string>>>
            match(m + 1, std::vector<std::pair<std::string, std::string>> (n + 1));
    match[0][0] = {"", ""};
    for (std::size_t i = 1; i <= m; ++i) {
        P[i][0] = -c_0 - c_1 * static_cast<double>(i);
        match[i][0] = match[i - 1][0];
        match[i][0].first += x[i - 1];
        match[i][0].second += '-';
    }
    for (std::size_t j = 1; j <= n; ++j) {
        P[0][j] = -c_0 - c_1 * static_cast<double>(j);
        match[0][j] = match[0][j - 1];
        match[0][j].first += '-';
        match[0][j].second += y[j - 1];
    }
    for (std::size_t i = 1; i <= m; ++i) {
        for (std::size_t j = 1; j <= n; ++j) {
            F[i][j] = P[i - 1][j - 1] + delta[charIndices[x[i - 1]]][charIndices[y[j - 1]]];
            G[i][j] = std::max(P[i - 1][j] - c_0 - c_1, G[i - 1][j] - c_1);
            H[i][j] = std::max(P[i][j - 1] - c_0 - c_1, H[i][j - 1] - c_1);
            auto val = std::max({F[i][j], G[i][j], H[i][j]});
            P[i][j] = val;
            if (val == F[i][j]) {
                match[i][j] = match[i - 1][j - 1];
                match[i][j].first += x[i - 1];
                match[i][j].second += y[j - 1];
            } else if (val == G[i][j]) {
                match[i][j] = match[i - 1][j];
                match[i][j].first += x[i - 1];
                match[i][j].second += '-';
            } else if (val == H[i][j]) {
                match[i][j] = match[i][j - 1];
                match[i][j].first += '-';
                match[i][j].second += y[j - 1];
            } else {
                assert(0);
            }
        }
    }

    return match[m][n];
}

int main() {
    // (4 + 1) x (4 x 1)
    std::vector<std::vector<double>> delta {{1, -1, -1, -1},
                                         {-1, 1, -1, -1},
                                         {-1, -1, 1, -1},
                                         {-1, -1, -1, 1}};
    auto [x1, y1] = maxAlignment("ATGCC", "TACGCA", delta, 3.0, 2.0);
    std::cout << x1 << ' ' << y1 << '\n';

}