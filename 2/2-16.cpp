#include <algorithm>
#include <cassert>
#include <cstddef>
#include <concepts>
#include <ranges>

template <std::ranges::random_access_range R> requires std::regular<std::iter_reference_t<std::ranges::iterator_t<R>>>
std::size_t search(R&& rang, const std::iter_reference_t<std::ranges::iterator_t<R>>& target) {
    using T = std::iter_reference_t<std::ranges::iterator_t<R>>;
    if (*std::begin(rang) == target) {
        return 0;
    }
    std::size_t l = 0;
    std::size_t r = 1;
    while (r < rang.size() && std::begin(rang) + r < target) {
        l = r;
        r <<= 1;
    }
    std::size_t pivot = 0;
    T num {0};
    r >>= 1;
    while (l <= r) {
        pivot = l + ((r - l) >> 1);
        num = rang[pivot];
        if (num == target) {
            return pivot;
        } else if (num < target) {
            r = pivot - 1;
        } else {
            l = pivot + 1;
        }
    }
    return -1;
}

int main() {
}