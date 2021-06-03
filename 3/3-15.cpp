#include <cassert>
#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <stack>
#include <list>
#include <unordered_map>

enum class Color {
    White,
    Gray,
    Black
};

using info_t = std::tuple<Color, std::size_t, std::size_t, std::size_t>;

template <std::size_t n>
class Graph {
    std::vector<std::vector<std::size_t>> adj;
public:
    Graph() : adj(n) {
    }

    std::vector<info_t> infos;

    void addEdge(std::size_t src, std::size_t dst) {
        assert(src < n && dst < n);
        adj[src].push_back(dst);
    }

    [[nodiscard]] std::vector<std::vector<std::size_t>> getStronglyConnectedComponents() {
        infos = std::vector<info_t>(n, std::make_tuple(Color::White, 0, 0, -1));
        std::size_t time = 0;
        std::stack<std::size_t> S;
        std::list<std::size_t> toVisit;
        for (std::size_t u = 0; u < n; u++) {
            if (std::get<0>(infos[u]) == Color::White) {
                S.push(u);
                while (!S.empty()) {
                    auto v = S.top();
                    time++;
                    std::get<0>(infos[v]) = Color::Gray;
                    std::get<1>(infos[v]) = time;
                    bool finished = true;
                    for (auto w : adj[v]) {
                        if (std::get<0>(infos[w]) == Color::White) {
                            S.push(w);
                            finished = false;
                        }
                    }
                    if (finished) {
                        S.pop();
                        time++;
                        std::get<0>(infos[v]) = Color::Black;
                        std::get<2>(infos[v]) = time;
                        toVisit.push_front(v);
                    }
                }
            }
        }

        Graph transposed;

        for (std::size_t i = 0; i < n; i++) {
            for (auto neighbor : adj[i]) {
                transposed.adj[neighbor].push_back(i);
            }
        }

        std::vector<std::vector<std::size_t>> SCCs;

        transposed.infos = std::vector<info_t>(n, std::make_tuple(Color::White, 0, 0, -1));
        time = 0;
        for (auto u : toVisit) {
            if (std::get<0>(transposed.infos[u]) == Color::White) {
                std::vector<std::size_t> SCC;
                S.push(u);
                SCC.push_back(u);
                while (!S.empty()) {
                    auto v = S.top();
                    time++;
                    std::get<0>(transposed.infos[v]) = Color::Gray;
                    std::get<1>(transposed.infos[v]) = time;
                    bool finished = true;
                    for (auto w : transposed.adj[v]) {
                        if (std::get<0>(transposed.infos[w]) == Color::White) {
                            S.push(w);
                            SCC.push_back(w);
                            finished = false;
                        }
                    }
                    if (finished) {
                        S.pop();
                        time++;
                        std::get<0>(transposed.infos[v]) = Color::Black;
                        std::get<2>(transposed.infos[v]) = time;
                    }
                }
                SCCs.push_back(SCC);
            }
        }
        return SCCs;
    }

    bool isPartATrue() {
        return getStronglyConnectedComponents().size() == 1;
    }

    bool isPartBTrue(std::size_t src) {
        auto SCCs = getStronglyConnectedComponents();
        std::unordered_map<std::size_t, std::size_t> SCCIndex;
        for (std::size_t i = 0; i < SCCs.size(); ++i) {
            for (auto node : SCCs[i]) {
                SCCIndex[node] = i;
            }
        }
        std::size_t cur_scc = SCCIndex[src];

        std::vector<int> visited (n);
        std::stack<std::size_t> S;
        S.push(src);
        while (!S.empty()) {
            auto v = S.top();
            if (SCCIndex[v] != cur_scc) {
                return false;
            }
            visited[v] = true;
            bool finished = true;
            for (auto w : adj[v]) {
                if (!visited[w]) {
                    S.push(w);
                    finished = false;
                }
            }
            if (finished) {
                S.pop();
            }
        }
        return true;
    }

};


int main() {


}
