#include <algorithm>
#include <cstddef>
#include <vector>

template <typename T>
void deduplicate(std::vector<T>& v) {
    auto ret = std::ranges::unique(v);
    v.erase(ret.begin(), ret.end());
}

int main() {
}