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
                                                   {'G', 3},
                                                   {'-', 4}};

std::pair<std::string, std::string> maxAlignment(const std::string& x, const std::string& y,
                                                 const std::vector<std::vector<double>>& delta) {
    const std::size_t m = x.length();
    const std::size_t n = y.length();

    std::vector<std::vector<double>> P(m + 1, std::vector<double>(n + 1));
    std::vector<std::vector<std::pair<std::string, std::string>>>
            match(m + 1, std::vector<std::pair<std::string, std::string>> (n + 1));
    match[0][0] = {"", ""};
    for (std::size_t i = 1; i <= m; ++i) {
        P[i][0] = P[i - 1][0] + delta[charIndices[x[i - 1]]][charIndices['-']];
        match[i][0] = match[i - 1][0];
        match[i][0].first += x[i - 1];
        match[i][0].second += '-';
    }
    for (std::size_t j = 1; j <= n; ++j) {
        P[0][j] = P[0][j - 1] + delta[charIndices['-']][charIndices[y[j - 1]]];
        match[0][j] = match[0][j - 1];
        match[0][j].first += '-';
        match[0][j].second += y[j - 1];
    }
    for (std::size_t i = 1; i <= m; ++i) {
        for (std::size_t j = 1; j <= n; ++j) {
            auto val1 = P[i - 1][j - 1] + delta[charIndices[x[i - 1]]][charIndices[y[j - 1]]];
            auto val2 = P[i - 1][j] + delta[charIndices[x[i - 1]]][charIndices['-']];
            auto val3 = P[i][j - 1] + delta[charIndices['-']][charIndices[y[j - 1]]];
            auto val = std::max({val1, val2, val3});
            P[i][j] = val;
            if (val == val1) {
                match[i][j] = match[i - 1][j - 1];
                match[i][j].first += x[i - 1];
                match[i][j].second += y[j - 1];
            } else if (val == val2) {
                match[i][j] = match[i - 1][j];
                match[i][j].first += x[i - 1];
                match[i][j].second += '-';
            } else if (val == val3) {
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
    std::vector<std::vector<double>> delta {{1, -1, -1, -1, -2},
                                         {-1, 1, -1, -1, -2},
                                         {-1, -1, 1, -1, -2},
                                         {-1, -1, -1, 1, -2},
                                         {-2, -2, -2, -2, 0}};
    auto [x1, y1] = maxAlignment("ATGCC", "TACGCA", delta);
    std::cout << x1 << ' ' << y1 << '\n';

}