#include <algorithm>
#include <vector>

bool prescribedDegreeGraphExists(const std::vector<std::size_t>& degs) {
    if (degs.empty()) {
        return true;
    }
    auto new_degs = degs;
    std::ranges::sort(new_degs);
    while (true) {
        if (new_degs.size() == 1) {
            return new_degs.back() == 0;
        } else {
            if (new_degs.back() >= new_degs.size()) {
                return false;
            }
            auto largest_deg = new_degs.back();
            new_degs.pop_back();
            for (std::size_t i = 0; i < largest_deg; --i) {
                --new_degs[new_degs.size() - 1 - i];
            }
        }
    }
    return false;
}

int main() {

}
