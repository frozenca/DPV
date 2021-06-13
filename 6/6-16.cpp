#include <cstddef>
#include <cassert>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <limits>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <optional>

double realGen() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<> real_dist(1.0, 20.0);
    return real_dist(gen);
}

std::pair<std::vector<std::size_t>, double> getMaxValue(const std::vector<double>& vals,
                                             const std::vector<std::vector<double>>& dist_costs) {
    std::tuple<std::uint16_t, std::size_t, double> ans {0, -1, std::numeric_limits<double>::lowest()};
    const std::size_t n = vals.size();
    assert(n < 16);
    std::unordered_map<std::uint16_t, std::vector<double>> valueMap;
    valueMap[0].resize(n);
    for (std::size_t j = 0; j < n; ++j) {
        valueMap[0][j] = vals[j] - dist_costs[0][j + 1];
    }

    std::vector<std::unordered_set<std::uint16_t>> subsets (n + 1);
    // warning : n = 16 will cause (defined) infinite loop!
    for (std::uint16_t index = 0; index <= static_cast<std::uint16_t>((1u << n) - 1); ++index) {
        int cnt = std::popcount(index);
        subsets[cnt].insert(index);
    }

    // optimal back element to extend to unvisited j
    std::unordered_map<std::uint16_t, std::vector<std::size_t>> optimalBack;

    for (std::size_t k = 1; k < n; ++k) {
        for (auto subset_k : subsets[k]) {
            optimalBack[subset_k].resize(n);
            valueMap[subset_k].resize(n);
            for (std::size_t j = 0; j < n; ++j) {
                if (!(subset_k & (1u << j))) { // extend to unvisited j.
                    std::size_t max_i = -1;
                    double valueMap_subsetk_j = std::numeric_limits<double>::lowest();
                    for (std::size_t i = 0; i < n; ++i) {
                        if (subset_k & (1u << i)) { // backtrack the last visited.
                            auto subset_k_minus_i = subset_k & ~(1u << i);
                            double value_from_i_to_j = valueMap[subset_k_minus_i][i] + vals[j] - dist_costs[i + 1][j + 1];
                            if (valueMap_subsetk_j < value_from_i_to_j) {
                                valueMap_subsetk_j = value_from_i_to_j;
                                max_i = i;
                            }
                        }
                    }
                    valueMap[subset_k][j] = valueMap_subsetk_j;
                    assert(max_i != -1);
                    optimalBack[subset_k][j] = max_i;
                    auto ans_candidate = valueMap_subsetk_j - dist_costs[j + 1][0];
                    if (std::get<2>(ans) < ans_candidate) {
                        ans = {subset_k | (1u << j), j, ans_candidate};
                    }
                }
            }
        }
    }

    auto [optimalSet, optimalLast, optimalVal] = ans;
    std::vector<std::size_t> optimalSequence;
    while (true) {
        optimalSequence.push_back(optimalLast);
        optimalSet = optimalSet & ~(1u << optimalLast);
        if (!optimalSet) {
            break;
        }
        auto optimalSecondLast = optimalBack[optimalSet][optimalLast];
        optimalLast = optimalSecondLast;
    }
    std::ranges::reverse(optimalSequence);
    return {optimalSequence, optimalVal};
}

int main() {
    constexpr std::size_t n = 15;
    std::vector<double> vals (n);
    std::vector<std::vector<double>> dist_costs(n + 1, std::vector<double>(n + 1));
    std::ranges::generate(vals, realGen);
    for (auto& dist_cost : dist_costs) {
        std::ranges::generate(dist_cost, realGen);
    }
    auto [route, max_val] = getMaxValue(vals, dist_costs);
    for (auto index : route) {
        std::cout << index << ' ';
    }
    std::cout << '\n';
    std::cout << max_val << '\n';

}