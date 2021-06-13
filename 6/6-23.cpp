#include <cstddef>
#include <cassert>
#include <vector>
#include <utility>
#include <algorithm>
#include <limits>
#include <cmath>
#include <random>
#include <iostream>

std::pair<std::vector<std::size_t>, double> maxReliablity(const std::vector<std::pair<double, std::size_t>>& probCosts,
                                                          std::size_t B) {
    const std::size_t n = probCosts.size();
    assert(n);
    std::vector<std::vector<std::pair<double, std::size_t>>>
            R(n + 1, std::vector<std::pair<double, std::size_t>>(B + 1));
    std::ranges::fill(R[0], std::make_pair(1.0, 0));
    for (std::size_t i = 1; i <= n; ++i) {
        auto& [r_i, c_i] = probCosts[i - 1];
        for (std::size_t b = 0; b <= B; ++b) {
            std::size_t max_k = b / c_i;
            double curr_prob = std::numeric_limits<double>::lowest();
            std::size_t curr_redundancy = -1;
            for (std::size_t k = 0; k <= max_k; ++k) {
                double k_prob = R[i - 1][b - k * c_i].first * (1.0 - std::pow(1 - r_i, k));
                if (curr_prob < k_prob) {
                    curr_prob = k_prob;
                    curr_redundancy = k;
                }
            }
            R[i][b] = {curr_prob, curr_redundancy};
        }
    }
    auto max_reliablity = R[n][B].first;
    std::vector<std::size_t> optimal_redundancies (n);
    auto curr_b = B;
    for (std::size_t i = n - 1; i < n; --i) {
        optimal_redundancies[i] = R[i + 1][curr_b].second;
        curr_b -= optimal_redundancies[i] * probCosts[i].second;
    }
    return {optimal_redundancies, max_reliablity};
}

int main() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> prob_dist(0.5, 0.99);
    std::uniform_int_distribution<> cost_dist(10, 100);
    constexpr std::size_t n = 20;
    std::vector<std::pair<double, std::size_t>> probCosts;
    for (std::size_t i = 0; i < n; ++i) {
        probCosts.emplace_back(prob_dist(gen), cost_dist(gen));
    }
    auto budget = cost_dist(gen) * n * 3;
    auto [redundancies, rel] = maxReliablity(probCosts, budget);
    for (auto red : redundancies) {
        std::cout << red << ' ';
    }
    std::cout << '\n';
    std::cout << rel << '\n';


}