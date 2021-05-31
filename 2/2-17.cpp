#include <algorithm>
#include <cassert>
#include <cstddef>
#include <concepts>
#include <ranges>

template <typename T>
std::size_t search(const std::vector<T>& v) {
    assert(!v.empty());
    std::size_t l = 0;
    std::size_t r = v.size() - 1;
    while (l <= r) {
        auto m = l + (r - l) / 2;
        if (v[m] == m) {
            return m;
        } else if (v[m] < m) {
            r = m - 1;
        } else {
            l = m + 1;
        }
    }
    return -1;
}

int main() {
}