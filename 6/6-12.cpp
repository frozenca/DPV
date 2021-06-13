#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>
#include <random>
#include <set>
#include <vector>
#include <ranges>

namespace sr = std::ranges;

struct Point2d {
    double x = 0.0;
    double y = 0.0;
    Point2d() = default;
    Point2d(double x, double y) : x {x}, y {y} {}
};

double dist(const Point2d& lhs, const Point2d& rhs) {
    return std::hypot(lhs.x - rhs.x, lhs.y - rhs.y);
}

double cost(const Point2d& i, const Point2d& j, const Point2d& k) {
    return dist(i, j) + dist(j, k) + dist(k, i);
}

double minimumTriangulation(const std::vector<Point2d>& points,
                            std::vector<std::vector<double>>& memo,
                            std::size_t i, std::size_t j) {
    double& res = memo[i][j];
    assert(i <= j);
    if (j < i + 2) {
        res = 0;
        return res;
    } else if (res != std::numeric_limits<double>::max()) {
        return res;
    }
    for (std::size_t k = i + 1; k < j; ++k) {
        res = std::min(res, minimumTriangulation(points, memo, i, k) + minimumTriangulation(points, memo, k, j)
        + cost(points[i], points[j], points[k]));
    }
    return res;
}

double minimumTriangulation(const std::vector<Point2d>& points) {
    const std::size_t n = points.size();
    assert(n);
    std::vector<std::vector<double>> memo(n, std::vector<double>(n, std::numeric_limits<double>::max()));
    minimumTriangulation(points, memo, 0, n - 1);
    return memo[0][n - 1];
}

int main() {
    std::vector<Point2d> Q {{0, 0}, {1, 0}, {2, 1}, {1, 2}, {0, 2}};
    std::cout << minimumTriangulation(Q);


}