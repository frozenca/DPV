#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>
#include <vector>

std::vector<std::size_t> minimumPenaltyPath(const std::vector<double>& hotels) {
    const std::size_t n = hotels.size();
    std::vector<std::size_t> path (n + 1);
    std::vector<double> min_penalty (n + 1);
    for (std::size_t i = 1; i <= n; ++i) {
        min_penalty[i] = std::numeric_limits<double>::max();
        for (std::size_t j = 0; j < i; ++j) {
            auto curr_penalty = min_penalty[j] + std::pow(200 - hotels[i - 1] + hotels[j - 1], 2);
            if (min_penalty[i] > curr_penalty) {
                path[i] = j;
                min_penalty[i] = curr_penalty;
            }
        }
    }
    std::vector<std::size_t> optimal_path;
    auto curr = n;
    while (curr) {
        optimal_path.push_back(curr);
        curr = path[curr];
    }
    std::ranges::reverse(optimal_path);
    return optimal_path;
}

int main() {
    auto path1 = minimumPenaltyPath({200, 400, 600});
    auto path2 = minimumPenaltyPath({200, 400, 600, 601});
    for (auto p : path1) {
        std::cout << p << ' ';
    }
    std::cout << '\n';
    for (auto p : path2) {
        std::cout << p << ' ';
    }
    std::cout << '\n';
}
