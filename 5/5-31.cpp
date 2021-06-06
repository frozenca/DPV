#include <cassert>
#include <vector>
#include <queue>
#include <functional>

template <typename T>
std::priority_queue<T> Huffman(const std::vector<T>& C, const std::vector<T>& L) {
    assert(!C.empty());
    std::size_t n = C.size();
    std::vector<T> CL (n);
    for (std::size_t i = 0; i < n; ++i) {
        CL[i] = C[i] * L[i];
    }
    std::priority_queue<T, std::vector<T>, std::greater<>> Q (CL.begin(), CL.end());
    for (size_t i = 0; i < n - 1; i++) {
        auto x = Q.top();
        Q.pop();
        auto y = Q.top();
        Q.pop();
        auto z = x + y;
        Q.push(z);
    }
    return Q;
}

int main() {
}
