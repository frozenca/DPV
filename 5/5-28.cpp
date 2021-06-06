#include <cassert>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <list>
#include <set>
#include <unordered_set>
#include <random>

template <std::size_t n>
class Graph {
    std::vector<std::list<std::size_t>> adj;
    std::unordered_map<std::size_t, std::list<std::size_t>::iterator> edges;

public:
    Graph() : adj(n) {
    }

    void addEdge(std::size_t src, std::size_t dst) {
        assert(src < n && dst < n);
        if (!edges.contains(src * n + dst)) {
            adj[src].emplace_front(dst);
            edges[src * n + dst] = adj[src].begin();
        }
        if (!edges.contains(dst * n + src)) {
            adj[dst].emplace_front(src);
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

    [[nodiscard]] std::unordered_set<std::size_t> getInvitees() {
        std::vector<std::size_t> know_num (n);
        std::vector<std::size_t> unknow_num (n, n - 1);
        for (std::size_t i = 0; i < n; ++i) {
            for (auto j : adj[i]) {
                know_num[i]++;
                know_num[j]++;
                unknow_num[i]--;
                unknow_num[j]--;
            }
        }
        std::set<std::pair<std::size_t, std::size_t>> know_set;
        std::set<std::pair<std::size_t, std::size_t>> unknow_set;
        for (std::size_t i = 0; i < n; ++i) {
            know_set.emplace(know_num[i], i);
            unknow_set.emplace(unknow_num[i], i);
        }

        while (true) {
            if (!know_set.empty()) {
                auto [least_know, idx] = *know_set.begin();
                if (least_know < 5) { // kick outsider
                    for (std::size_t i = 0; i < n; ++i) {
                        if (edges.contains(idx * n + i)) {
                            know_set.erase({know_num[i], i});
                            know_num[i]--;
                            know_set.emplace(know_num[i], i);
                        } else {
                            unknow_set.erase({unknow_num[i], i});
                            unknow_num[i]--;
                            unknow_set.emplace(unknow_num[i], i);
                        }
                    }
                    know_set.erase({least_know, idx});
                    unknow_set.erase({unknow_num[idx], idx});
                    continue;
                }
            }
            if (!unknow_set.empty()) {
                auto [least_unknow, idx] = *unknow_set.begin();
                if (least_unknow < 5) { // kick insider
                    for (std::size_t i = 0; i < n; ++i) {
                        if (edges.contains(idx * n + i)) {
                            know_set.erase({know_num[i], i});
                            know_num[i]--;
                            know_set.emplace(know_num[i], i);
                        } else {
                            unknow_set.erase({unknow_num[i], i});
                            unknow_num[i]--;
                            unknow_set.emplace(unknow_num[i], i);
                        }
                    }
                    know_set.erase({know_num[idx], idx});
                    unknow_set.erase({least_unknow, idx});
                    continue;
                }
            }
            break;
        }
        std::unordered_set<std::size_t> invitees;
        for (const auto& [cnt, idx] : know_set) {
            invitees.insert(idx);
        }
        return invitees;
    }
};

int main() {
    Graph<20> g;
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution dist(0, 19);
    for (std::size_t e = 0; e < 30; ++e) {
        auto a = dist(gen);
        auto b = dist(gen);
        if (a != b) {
            std::cout << a << '-' << b << '\n';
            g.addEdge(a, b);
        }
    }
    auto inv = g.getInvitees();

    for (auto memb : inv) {
        std::cout << memb << ' ';
    }
    std::cout << '\n';


}
