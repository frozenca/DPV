#include <bit>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <optional>
#include <tuple>

std::tuple<std::size_t, int, int> ExtendedEuclid(std::size_t a, std::size_t b) {
    if (!b) {
        return {a, 1, 0};
    } else {
        auto [d_, x_, y_] = ExtendedEuclid(b, a % b);
        std::size_t d = d_;
        int x = y_;
        int y = x_ - static_cast<int>(a / b) * y_;
        return {d, x, y};
    }
}

std::size_t mod(int a, std::size_t n) {
    if (a >= 0) {
        return a % n;
    } else {
        return n - ((-a) % n);
    }
}

std::optional<std::size_t> ModularInverse(std::size_t a, std::size_t n) {
    auto [d, x_, y_] = ExtendedEuclid(a, n);
    if (d == 1) {
        return mod(x_ / static_cast<int>(d), n);
    } else {
        return {};
    }
}

std::size_t ModularExponentiation(std::size_t a, std::size_t b, std::size_t n) {
    assert(n);
    std::size_t c = 0;
    std::size_t d = 1;
    auto k = std::bit_width(b);
    std::bitset<64> B(b);
    for (std::size_t i = k - 1; i < k; i--) {
        c *= 2;
        d = (d * d) % n;
        if (B[i]) {
            c++;
            d = (d * a) % n;
        }
    }
    return d;
}

std::size_t Crack(std::size_t p, std::size_t e, std::size_t memodp) {
    auto d = *ModularInverse(e, p - 1);
    return ModularExponentiation(memodp, d, p);
}

int main() {

}