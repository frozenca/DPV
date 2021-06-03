#include <cassert>
#include <iostream>
#include <vector>
#include <utility>
#include <tuple>

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

    bool DFSVisit(std::size_t u, std::size_t length) {
        std::get<0>(infos[u]) = Color::Gray;
        for (auto v : adj[u]) {
            if (std::get<0>(infos[v]) == Color::White) {
                std::get<3>(infos[v]) = u;
                DFSVisit(v, ++length);
            } else if (std::get<0>(infos[v]) == Color::Gray) {
                return true;
            }
        }
        std::get<0>(infos[u]) = Color::Black;
        return false;
    }

    bool hasCycle() {
        infos = std::vector<info_t>(n, std::make_tuple(Color::White, 0, 0, -1));
        for (std::size_t i = 0; i < n; i++) {
            if (std::get<0>(infos[i]) == Color::White) {
                if (DFSVisit(i, 1)) {
                    return true;
                }
            }
        }
        return false;
    }
};


int main() {

}
