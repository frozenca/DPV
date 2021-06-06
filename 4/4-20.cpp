#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <concepts>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <ranges>
#include <stack>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

namespace sr = std::ranges;
namespace srv = std::ranges::views;
namespace crn = std::chrono;

template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept hashable = requires (T a) {
    {std::hash<T>{}(a)} -> std::convertible_to<std::size_t>;
};

constexpr std::size_t V = 1'000;
constexpr std::size_t W = 3;

std::mt19937 gen(std::random_device{}());
std::uniform_int_distribution<> dist(1, std::pow(2, W) - 1);
std::bernoulli_distribution d(0.01);

std::array<std::size_t, V * V> w = {0};
std::array<std::size_t, V * V> curr_w = {0};

template <std::size_t n, arithmetic T = double, bool undirected = false>
class Graph {
    std::vector<std::list<std::pair<std::size_t, T>>> adj;
    std::unordered_map<std::size_t, typename std::list<std::pair<std::size_t, T>>::iterator> edges;

public:
    Graph() : adj(n) {
    }

    void addEdge(std::size_t src, std::size_t dst, T weight) {
        assert(src < n && dst < n);
        if (!edges.contains(src * n + dst)) {
            adj[src].emplace_front(dst, weight);
            edges[src * n + dst] = adj[src].begin();
        }
        if (undirected) {
            if (!edges.contains(dst * n + src)) {
                adj[dst].emplace_front(src, weight);
                edges[dst * n + src] = adj[dst].begin();
            }
        }
    }

    void removeEdge(std::size_t src, std::size_t dst) {
        assert(src < n && dst < n);
        adj[src].erase(edges[src * n + dst]);
        edges.erase(src * n + dst);
        if (undirected) {
            adj[dst].erase(edges[dst * n + src]);
            edges.erase(dst * n + src);
        }
    }

    [[nodiscard]] T getWeight(std::size_t src, std::size_t dst) const {
        assert(src < n && dst < n);
        if (!edges.contains(src * n + dst)) {
            return T {0};
        } else {
            return edges.at(src * n + dst)->second;
        }
    }

    using Edge = std::tuple<std::size_t, std::size_t, T>;

    std::vector<T> DijkstraPQ(std::size_t s) {
        std::priority_queue<std::pair<T, std::size_t>, std::vector<std::pair<T, std::size_t>>, std::greater<>> Q;
        Q.emplace(0.0, s);

        std::vector<T> dists (n, std::numeric_limits<T>::max());
        dists[s] = T{0};

        while (!Q.empty()) {
            auto [d, u] = Q.top();
            Q.pop();
            for (const auto& [v, w_] : adj[u]) {
                if (dists[v] > dists[u] + w_) {
                    dists[v] = dists[u] + w_;
                    Q.emplace(dists[v], v);
                }
            }
        }
        return dists;
    }

    Edge decreaseMaximizer(const std::vector<Edge>& edge_candidates, std::size_t s, std::size_t t) {
        auto d_s = DijkstraPQ(s);
        auto d_t = DijkstraPQ(t);
        return *std::ranges::min_element(edge_candidates, [&](const auto& edge1, const auto& edge2) {
            auto [x1, y1, w1] = edge1;
            auto [x2, y2, w2] = edge2;
            return d_s[x1] + w1 + d_t[y1] < d_s[x2] + w2 + d_t[y2];
        });
    }


};

int main() {

}