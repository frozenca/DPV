#include <array>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <concepts>
#include <iostream>
#include <limits>
#include <ranges>
#include <type_traits>
#include <utility>

namespace sr = std::ranges;

template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template <std::size_t n, arithmetic T = double, bool undirected = false>
class Graph {
    std::array<T, n * n> W;

public:
    Graph() {
        sr::fill(W, std::numeric_limits<T>::max());
    }

    void addEdge(std::size_t src, std::size_t dst, T weight) {
        assert(src < n && dst < n);
        W[src * n + dst] = weight;
        if (undirected) {
            W[dst * n + src] = weight;
        }
    }

    void removeEdge(std::size_t src, std::size_t dst) {
        addEdge(src, dst, std::numeric_limits<T>::max());
    }

    [[nodiscard]] T getWeight(std::size_t src, std::size_t dst) const {
        return W[src * n + dst];
    }

    [[nodiscard]] T shortestCycle() const {
        auto D = W;
        for (std::size_t k = 0; k < n; k++) {
            std::array<T, n * n> D_;
            for (std::size_t i = 0; i < n; i++) {
                for (std::size_t j = 0; j < n; j++) {
                    D_[i * n + j] = std::min(D[i * n + j], D[i * n + k] + D[k * n + j]);
                }
            }
            std::swap(D, D_);
        }
        T min_length = std::numeric_limits<T>::max();
        for (std::size_t i = 0; i < n; ++i) {
            min_length = std::min(min_length, D[i * n + i]);
        }
        return min_length;
    }


};

int main() {




}