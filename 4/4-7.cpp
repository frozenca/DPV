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

    bool isShortestPathTree(const Graph& g, std::size_t src) {
        std::vector<T> distances (n);
        distances[src] = T{0};
        std::deque<std::size_t> q;
        q.push_back(src);
        while (!q.empty()) {
            auto curr = q.front();
            q.pop_front();
            // assumes g is tree.
            for (const auto& [neigh, weight] : g.adj[curr]) {
                distances[neigh] = distances[curr] + weight;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            for (const auto& [dst, weight] : adj[i]) {
                if (distances[i] + weight < distances[dst]) {
                    return false;
                }
            }
        }
        return true;
    }

};

int main() {

}