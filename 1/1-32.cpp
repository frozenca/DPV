#include <cassert>
#include <cmath>

bool is_power(std::size_t N) {
    if (N == 1) {
        return true;
    }
    auto logN = static_cast<std::size_t>(std::floor(std::log(N)));
    for (std::size_t k = 2; k <= logN; ++k) {
        std::size_t l = 2;
        std::size_t r = N / k;
        while (l < r) {
            auto m = l + (r - l) / 2;
            auto mk = static_cast<std::size_t>(std::pow(m, k));
            if (mk == N) {
                return true;
            } else if (mk < N) {
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
    }
    return false;
}

int main() {
    assert(is_power(2401));
    assert(!is_power(2402));

}
