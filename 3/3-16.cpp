#include <cassert>
#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <stack>

template <std::size_t n>
class Graph {
    std::vector<std::vector<std::size_t>> adj;
public:
    Graph() : adj(n) {
    }

    std::size_t TopologicalSort() {
        std::stack<std::pair<std::size_t, std::size_t>> firstNodes;
        std::vector<std::size_t> topSort;
        std::vector<int> indegrees(n);
        for (std::size_t src = 0; src < n; ++src) {
            for (auto dst : adj[src]) {
                ++indegrees[dst];
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            if (!indegrees[i]) {
                firstNodes.emplace(i, 1);
            }
        }
        std::size_t cnt = 1;
        while (!firstNodes.empty()) {
            auto [curr, curr_length] = firstNodes.top();
            firstNodes.pop();
            topSort.push_back(curr);
            for (auto dst : adj[curr]) {
                --indegrees[dst];
                if (!indegrees[dst]) {
                    firstNodes.emplace(dst, curr_length + 1);
                    cnt = std::max(cnt, curr_length + 1);
                }
            }
        }

        return cnt;
    }
};


int main() {

}
