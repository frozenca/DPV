#include <cassert>
#include <cstddef>
#include <concepts>
#include <optional>
#include <vector>

template <std::semiregular T>
std::optional<T> majorityElement(const std::vector<T>& elems) {
    assert(!elems.empty());
    std::size_t count = 0;
    std::optional<T> candidate;

    for (const auto& elem : elems) {
        if (!count) {
            candidate = elem;
        }
        count += (*candidate == elem) ? 1 : -1;
    }
    count = 0;
    for (const auto& elem : elems) {
        if (*candidate == elem) {
            ++count;
        }
    }
    if (count >= elems.size() / 2) {
        return candidate;
    } else {
        return {};
    }
}

int main() {
}