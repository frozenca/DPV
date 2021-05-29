#include <bit>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <random>
#include <numeric>
#include <iostream>

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

std::mt19937 gen(std::random_device{}());

std::pair<std::size_t, std::size_t> Crack(std::size_t N, std::size_t e, std::size_t d) {
    auto k = d * e - 1;
    std::uniform_int_distribution<> dist(2, N - 1);
    while (true) {
        auto g = dist(gen);
        auto t = k;
        if (t % 2) {
            continue;
        } else {
            t /= 2;
            auto x = ModularExponentiation(g, t, N);
            if (x > 1) {
                auto y = std::gcd(x - 1, N);
                if (y > 1) {
                    return {y, N / y};
                } else {
                    continue;
                }
            }
        }
    }
}

int main() {
    auto [p, q] = Crack(25777, 3, 16971);
    std::cout << p << ' ' << q << '\n';
}