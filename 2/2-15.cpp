#include <algorithm>
#include <cassert>
#include <cstddef>
#include <concepts>
#include <vector>

template <std::regular T>
void partition(std::vector<T>& A, const T& v) {
    assert(!A.empty());
    std::size_t i = 0;
    std::size_t j = 0;
    std::size_t k = A.size() - 1;
    while (j <= k) {
        if (A[j] < v) {
            std::swap(A[i], A[j]);
            ++i;
            ++j;
        } else if (A[j] > v) {
            std::swap(A[j], A[k]);
            --k;
        } else {
            ++j;
        }
    }
}

int main() {
}