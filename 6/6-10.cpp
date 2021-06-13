#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <execution>
#include <functional>
#include <iostream>
#include <vector>
#include <numbers>
#include <numeric>
#include <random>
#include <ranges>

namespace sr = std::ranges;
namespace srv = std::ranges::views;
namespace sn = std::numbers;

using Comp = std::complex<double>;
using Vec = std::vector<std::complex<double>>;

Vec PolyAdd(const Vec& A, const Vec& B) {
    std::size_t n = std::max(A.size(), B.size());
    Vec C (n);
    Vec A_ (n), B_ (n);
    sr::copy(A, A_.begin());
    sr::copy(B, B_.begin());
    sr::transform(A_, B_, C.begin(), std::plus{});
    while (!C.empty() && std::abs(C.back()) < 1e-9) {
        C.pop_back();
    }
    return C;
}

Vec PolySub(const Vec& A, const Vec& B) {
    std::size_t n = std::max(A.size(), B.size());
    Vec C (n);
    Vec A_ (n), B_ (n);
    sr::copy(A, A_.begin());
    sr::copy(B, B_.begin());
    sr::transform(A_, B_, C.begin(), std::minus{});
    while (!C.empty() && std::abs(C.back()) < 1e-9) {
        C.pop_back();
    }
    return C;
}

Vec PolyScale(const Vec& A, std::size_t n, bool div = false) {
    if (!div) {
        Vec A_ (A.size() + n);
        sr::copy(A, A_.begin() + n);
        return A_;
    } else {
        if (A.size() > n) {
            Vec A_ (A.begin() + n, A.end());
            return A_;
        } else {
            return Vec{0};
        }
    }
}

Vec RecursiveFFT(const Vec& A) {
    using namespace std::complex_literals;
    std::size_t n = A.size();
    if (n == 1) {
        return Vec {A[0]};
    }
    auto w_n = std::exp(2.0i * sn::pi / static_cast<double>(n));
    Comp w = 1;
    Vec A0 (n / 2);
    Vec A1 (n / 2);
    for (std::size_t i = 0; i < n; i += 2) {
        A0[i / 2] = A[i];
        A1[i / 2] = A[i + 1];
    }
    auto y0 = RecursiveFFT(A0);
    auto y1 = RecursiveFFT(A1);
    Vec y (n);
    for (std::size_t k = 0; k < n / 2; k++) {
        y[k] = y0[k] + w * y1[k];
        y[k + n / 2] = y0[k] - w * y1[k];
        w *= w_n;
    }
    return y;
}

Vec InverseDFT(const Vec& y) {
    using namespace std::complex_literals;
    std::size_t n = y.size();
    if (n == 1) {
        return Vec {y[0]};
    }
    auto w_n = std::exp(2.0i * sn::pi / (1.0 * n));
    Comp w = 1.0;
    Vec y0 (n / 2);
    Vec y1 (n / 2);
    for (std::size_t i = 0; i < n; i += 2) {
        y0[i / 2] = y[i];
        y1[i / 2] = y[i + 1];
    }
    auto a0 = InverseDFT(y0);
    auto a1 = InverseDFT(y1);
    Vec A (n);
    for (std::size_t k = 0; k < n / 2; k++) {
        A[k] = a0[k] + w * a1[k];
        A[k + n / 2] = a0[k] - w * a1[k];
        w *= (1.0 / w_n);
    }
    for (std::size_t k = 0; k < n; k++) {
        A[k] /= 2.0;
    }
    return A;
}

Vec PolynomialProduct(const Vec& A, const Vec& B) {
    std::size_t big_size = std::max(A.size(), B.size());
    std::size_t poly_size = (1u << (static_cast<std::size_t>(std::ceil(std::log2(big_size))) + 1));
    Vec A_poly = A;
    Vec B_poly = B;
    for (std::size_t k = 0; k < poly_size - A.size(); k++) {
        A_poly.push_back(0);
    }
    for (std::size_t k = 0; k < poly_size - B.size(); k++) {
        B_poly.push_back(0);
    }
    auto A_ = RecursiveFFT(A_poly);
    auto B_ = RecursiveFFT(B_poly);
    Vec AB_ (poly_size);
    sr::transform(A_, B_, AB_.begin(), std::multiplies{});

    auto AB = InverseDFT(AB_);
    while (!AB.empty() && std::abs(AB.back()) < 1e-8) {
        AB.pop_back();
    }
    return AB;
}

Vec computePoly(const std::vector<double>& probs, std::size_t i, std::size_t j) {
    assert(i <= j && j < probs.size());
    if (i == j) {
        return Vec{probs[i], 1.0 - probs[i]};
    }
    auto m = i + (j - i) / 2;
    auto left = computePoly(probs, i, m);
    auto right = computePoly(probs, m + 1, j);
    return PolynomialProduct(left, right);
}

double computeKHeads(const std::vector<double>& probs, std::size_t k) {
    const std::size_t n = probs.size();
    assert(!probs.empty() && k < n);
    auto poly = computePoly(probs, 0, n - 1);
    return std::real(poly[n - k]);
}

int main() {
    constexpr std::size_t n = 20;
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    std::vector<double> probs;
    for (std::size_t i = 0; i < n; ++i) {
        probs.push_back(dist(gen));
    }
    std::cout << computeKHeads(probs, 8);


}