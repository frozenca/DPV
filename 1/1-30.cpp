#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <ranges>

std::vector<int> add3 (const std::vector<int>& A,
                       const std::vector<int>& B,
                       const std::vector<int>& C) {
    const std::size_t n = A.size();
    assert(B.size() == n && C.size() == n);
    std::vector<int> carry (n + 1);
    std::vector<int> xored (n + 1);
    std::vector<int> res (n + 1);
    for (std::size_t i = 0; i < n; ++i) {
        carry[i] = (A[i] && (B[i] || C[i])) || (B[i] && C[i]);
        xored[i + 1] = A[i] ^ B[i] ^ C[i];
    }
    for (std::size_t i = n; i < n + 1; --i) {
        res[i] += carry[i] + xored[i];
        if (res[i] == 2) {
            res[i] -= 2;
            ++res[i - 1];
        }
    }
    return res;
}

std::vector<int> mul (const std::vector<int>& A,
                      const std::vector<int>& B) {
    const std::size_t n = A.size();
    assert(B.size() == n);
    std::vector<std::vector<int>> Vs;
    for (std::size_t i = 0; i < n; ++i) {
        if (B[i]) {
            std::vector<int> v (2 * n);
            std::ranges::copy(A, v.begin() + static_cast<long>(n - i));
            Vs.push_back(std::move(v));
        }
    }
    while (true) {
        const std::size_t m = Vs.size();
        std::vector<std::vector<int>> new_Vs;
        const std::size_t V = 3 * (m / 3);
        Vs.resize(V);
        for (std::size_t v = V - m; v < V; ++v) {
            Vs[v].resize(2 * n);
        }
        for (std::size_t v = 0; v < V; v += 3) {
            auto res = add3(Vs[v + 0], Vs[v + 1], Vs[v + 2]);
            new_Vs.push_back(std::move(res));
        }
        std::swap(new_Vs, Vs);
        if (Vs.size() == 1) {
            break;
        }
    }
    return Vs[0];
}

int main() {
    std::vector<int> A {1, 1, 0, 1, 0};
    std::vector<int> B {1, 0, 1, 0, 1};
    auto C = mul(A, B);
    for (auto c : C) {
        std::cout << c << ' ';
    }
}
