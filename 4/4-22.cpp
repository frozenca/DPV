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

    struct PathInfo {
        T d = std::numeric_limits<T>::max();
        std::size_t parent = -1;
    };

    void Relax(std::size_t u, std::size_t v, std::vector<PathInfo>& pinfos) {
        T w = getWeight(u, v);
        if (pinfos[v].d > pinfos[u].d + w) {
            pinfos[v].d = pinfos[u].d + w;
            pinfos[v].parent = u;
        }
    }

    std::pair<bool, std::vector<PathInfo>> BellmanFord(std::size_t s) {
        std::vector<PathInfo> pinfos;
        pinfos.resize(0);
        pinfos.resize(n);
        pinfos[s].d = T {0};
        for (std::size_t i = 0; i < n - 1; i++) {
            for (std::size_t u = 0; u < n; u++) {
                for (const auto& [v, w] : adj[u]) {
                    Relax(u, v, pinfos);
                }
            }
        }
        for (std::size_t u = 0; u < n; u++) {
            for (const auto& [v, w] : adj[u]) {
                if (pinfos[v].d > pinfos[u].d + w) {
                    return {false, pinfos};
                }
            }
        }
        return {true, pinfos};
    }

    bool DFSVisit(std::size_t u, std::vector<int>& visited, std::vector<std::size_t>& cycle) {
        visited[u] = 1;
        cycle.push_back(u);
        for (auto v : adj[u]) {
            if (!visited[v]) {
                DFSVisit(v, cycle);
            } else if (visited[v] == 1) {
                cycle.push_back(v);
                return true;
            }
        }
        visited[u] = 2;
        cycle.pop_back();
        return false;
    }

    std::pair<bool, std::vector<std::size_t>> hasCycle() {
        std::vector<int> visited(n);
        for (std::size_t i = 0; i < n; i++) {
            if (!visited[i]) {
                std::vector<std::size_t> cycle;
                if (DFSVisit(i, visited, cycle)) {
                    return {true, cycle};
                }
            }
        }
        return {false, {}};
    }

    std::pair<bool, std::vector<std::size_t>> ZeroCycleDetect(const std::vector<PathInfo>& pinfos) {
        Graph g;
        for (std::size_t i = 0; i < n; ++i) {
            for (auto& [j, w] : adj[i]) {
                if (pinfos[i] + w == pinfos[j]) {
                    g.addEdge(i, j, w);
                }
            }
        }
        return g.hasCycle();
    }

    std::vector<std::size_t> getMaxPCRCycle(const std::vector<T>& profits, const std::unordered_map<std::size_t, T>& costs,
                                            const T& eps) {
        T R = std::numeric_limits<T>::min();
        for (std::size_t i = 0; i < n; ++i) {
            for (const auto& [j, w] : adj[i]) {
                R = std::max(R, profits[j] / costs[i * n + j]);
            }
        }
        T lb = T{0};
        T rb = R;
        T r = lb + (rb - lb) / T{2};
        T temp_r = r;
        do {
            temp_r = r;
            for (std::size_t i = 0; i < n; ++i) {
                for (auto& [j, w] : adj[i]) {
                    w = temp_r - r * costs[i * n + j] - profits[j];
                }
            }
            auto [flag1, pinfos] = BellmanFord(0);
            if (!flag1) {
                lb = temp_r;
            } else {
                auto [flag2, Cycle] = ZeroCycleDetect(pinfos);
                if (flag2) {
                    return Cycle;
                } else {
                    rb = temp_r;
                }
            }
            r = lb + (rb - lb) / T{2};
        } while (std::abs(temp_r - r) > eps);
        return {};
    }



};

int main() {

}