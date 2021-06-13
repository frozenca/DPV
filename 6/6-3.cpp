#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>
#include <vector>
#include <utility>

double minimumPenaltyPath(const std::vector<std::pair<double, double>>& locationProfits_, double k) {
    auto locationProfits = locationProfits_;
    std::ranges::sort(locationProfits);
    const std::size_t n = locationProfits.size();
    std::vector<double> profits(n);
    profits[0] = locationProfits[0].second;
    for (std::size_t j = 1; j < n; ++j) {
        auto i = std::distance(locationProfits.begin(),
                               std::upper_bound(locationProfits.begin(), locationProfits.begin() + j,
                                   std::make_pair(locationProfits[j].first - k, 0),
                                   [](const auto& pair1, const auto& pair2){
                                       return pair1.first < pair2.first;
        }));
        profits[j] = std::max(profits[j - 1], (i == 0) ? 0 : profits[i - 1] + locationProfits[j].second);
    }
    return profits.back();
}

int main() {
    std::cout << minimumPenaltyPath({{1, 1.0}, {2, 4.0}, {3, 9.0}, {4, 16.0}}, 2);

}
