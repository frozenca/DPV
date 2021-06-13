#include <cstddef>
#include <cassert>
#include <tuple>
#include <vector>
#include <unordered_set>

struct Match {
    std::size_t l;
    std::size_t r;
    double w;
    Match(std::size_t l, std::size_t r, double w) : l{l}, r{r}, w{w} {
        assert(l <= r);
    }
};

std::vector<Match> bestMatches(std::size_t n,
        const std::vector<Match>& matches) {
    std::vector<double> W (n);
    std::vector<std::unordered_set<std::size_t>> matchesByRightEnd (n);
    for (std::size_t i = 0; i < matches.size(); ++i) {
        matchesByRightEnd[matches[i].r].insert(i);
    }
    std::vector<std::vector<Match>> optimalMatches (n);
    for (std::size_t i = 0; i < n; ++i) {
        W[i] = i ? W[i - 1] : 0.0;
        for (auto j : matchesByRightEnd[i]) {
            auto prev = matches[j].l - 1;
            auto curr_w = (prev < n) ? W[prev] : 0.0 + matches[j].w;
            if (W[i] < curr_w) {
                W[i] = curr_w;
                if (prev < n) {
                    optimalMatches[i] = optimalMatches[matches[j].l - 1];
                }
                optimalMatches[i].push_back(matches[j]);
            }
        }
    }
    return optimalMatches.back();
}

int main() {

}