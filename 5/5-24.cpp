#include <cassert>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <list>
#include <set>
#include <unordered_set>

template <std::size_t n>
class Graph {
    struct UnionInfo {
        std::size_t index;
        std::size_t parent;
        std::size_t rank = 0;
        UnionInfo(std::size_t index) : index {index}, parent {index} {}
    };

    std::vector<std::list<std::pair<std::size_t, double>>> adj;
    std::unordered_map<std::size_t, std::list<std::pair<std::size_t, double>>::iterator> edges;

    void Link(std::size_t x, std::size_t y) {
        auto& ux = uinfos[x];
        auto& uy = uinfos[y];
        if (ux.rank < uy.rank) {
            uy.parent = x;
        } else {
            ux.parent = y;
            if (ux.rank == uy.rank) {
                uy.rank++;
            }
        }
    }

    std::size_t FindSet(std::size_t x) {
        auto& ux = uinfos[x];
        if (ux.index != ux.parent) {
            ux.parent = FindSet(ux.parent);
        }
        return ux.parent;
    }

    void Union(std::size_t x, std::size_t y) {
        Link(FindSet(x), FindSet(y));
    }

public:
    Graph() : adj(n) {
        for (std::size_t i = 0; i < n; i++) {
            uinfos.emplace_back(i);
        }
    }

    std::vector<UnionInfo> uinfos;

    void addEdge(std::size_t src, std::size_t dst, double weight) {
        assert(src < n && dst < n);
        if (!edges.contains(src * n + dst)) {
            adj[src].emplace_front(dst, weight);
            edges[src * n + dst] = adj[src].begin();
        }
        if (!edges.contains(dst * n + src)) {
            adj[dst].emplace_front(src, weight);
            edges[dst * n + src] = adj[dst].begin();
        }
    }

    void removeEdge(std::size_t src, std::size_t dst) {
        assert(src < n && dst < n);
        adj[src].erase(edges[src * n + dst]);
        edges.erase(src * n + dst);
        adj[dst].erase(edges[dst * n + src]);
        edges.erase(dst * n + src);
    }

    using Edge = std::tuple<std::size_t, std::size_t, double>;

    std::vector<Edge> MSTKruskal() {
        std::vector<Edge> A;
        auto edge_comp = [](const auto& e1, const auto& e2) {
            return std::get<2>(e1) < std::get<2>(e2);
        };
        std::multiset<Edge, decltype(edge_comp)> sorted_edges(edge_comp);
        for (const auto& [idx, it] : edges) {
            std::size_t src = idx / n;
            std::size_t dst = idx % n;
            if (src > dst) {
                std::swap(src, dst); // ensure src < dst, no duplicate in set.
            }
            double w = it->second;
            sorted_edges.emplace(src, dst, w);
        }
        for (const auto& [src, dst, w] : sorted_edges) {
            if (FindSet(src) != FindSet(dst)) {
                A.emplace_back(src, dst, w);
                Union(src, dst);
            }
        }
        return A;
    }

    std::vector<Edge> getLightestSpanningTree(const std::unordered_set<std::size_t>& U) {
        uinfos.resize(0);
        for (std::size_t i = 0; i < n; i++) {
            uinfos.emplace_back(i);
        }
        Graph g;
        auto edge_comp = [](const auto& e1, const auto& e2) {
            return std::get<2>(e1) < std::get<2>(e2);
        };
        std::multiset<Edge, decltype(edge_comp)> E(edge_comp);
        for (const auto& [idx, it] : edges) {
            std::size_t src = idx / n;
            std::size_t dst = idx % n;
            if (src > dst) {
                std::swap(src, dst); // ensure src < dst, no duplicate in set.
            }
            double w = it->second;
            if (!U.contains(src) && !U.contains(dst)) {
                g.addEdge(src, dst, w);
            } else if (U.contains(src) && !U.contains(dst)) {
                E.emplace(src, dst, w);
            }
        }
        auto T = g.MSTKruskal();
        for (const auto& [src, dst, w] : E) {
            if (FindSet(src) != FindSet(dst)) {
                T.emplace_back(src, dst, w);
                Union(src, dst);
            }
        }
        return T;
    }
};

int main() {

}
