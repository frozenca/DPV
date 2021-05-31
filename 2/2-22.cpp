#include <cassert>
#include <cstddef>
#include <concepts>
#include <vector>

template <std::regular T>
T computeKth(const std::vector<T>& A, const std::vector<T>& B,
             std::size_t a1, std::size_t a2,
             std::size_t b1, std::size_t b2,
             std::size_t k) {
    if (a1 == a2) {
        return B[b1 + k];
    }
    if (b1 == b2) {
        return A[a1 + k];
    }
    std::size_t ma = (a2 - a1) / 2;
    std::size_t mb = (b2 - b1) / 2;
    if (ma + mb < k) {
        if (A[a1 + ma] > B[b1 + mb]) {
            return computeKth(A, B, a1, a2, b1 + mb + 1, b2, k - mb - 1);
        } else {
            return computeKth(A, B, a1 + ma + 1, a2, b1, b2, k - ma - 1);
        }
    } else {
        if (A[a1 + ma] > B[b1 + mb]) {
            return computeKth(A, B, a1, a1 + ma, b1, b2, k);
        } else {
            return computeKth(A, B, a1, a2, b1, b1 + mb, k);
        }
    }
}

int main() {
}