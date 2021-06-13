#include <cstddef>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <iostream>

using Product = std::tuple<std::size_t, std::size_t, double>;

static double computeC(std::unordered_map<std::size_t, double>& rect,
                     std::vector<std::vector<double>>& C,
                     std::size_t i,
                     std::size_t j) {
    double& ret = C[i - 1][j - 1];
    if (ret != std::numeric_limits<double>::lowest()) {
        return ret;
    }
    const std::size_t Y = C[0].size();
    if (i == 1) {
        ret = std::max(0.0, rect[1 * Y + j]);
        return ret;
    }
    if (j == 1) {
        ret = std::max(0.0, rect[i * Y + 1]);
        return ret;
    }
    auto max1 = std::numeric_limits<double>::lowest();
    auto max2 = std::numeric_limits<double>::lowest();
    auto max3 = rect[i * Y + j];
    for (std::size_t k = 1; k < i; ++k) {
        max1 = std::max(max1, computeC(rect, C, k, j) + computeC(rect, C, i - k, j));
    }
    for (std::size_t h = 1; h < j; ++h) {
        max2 = std::max(max2, computeC(rect, C, i, h) + computeC(rect, C, i, j - h));
    }
    ret = std::max({max1, max2, max3});
    return ret;
}

double getMaxProfit(const std::vector<Product>& prods, std::size_t X, std::size_t Y) {
    std::unordered_map<std::size_t, double> rect;
    for (const auto& [a, b, profit] : prods) {
        rect[a * Y + b] = std::max(rect[a * Y + b], profit);
    }
    std::vector<std::vector<double>> C(X, std::vector<double>(Y, std::numeric_limits<double>::lowest()));
    return computeC(rect, C, X, Y);
}

int main() {
    constexpr std::size_t X = 30;
    constexpr std::size_t Y = 40;

    std::vector<Product> products{{1, 2, 3},
                                  {4, 5, 6},
                                  {7, 8, 9}};

    std::cout << getMaxProfit(products, X, Y);

}