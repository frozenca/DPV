#include <algorithm>
#include <array>
#include <cassert>
#include <vector>
#include <random>
#include <iostream>

enum class chars {
    a,
    b,
    c,
};

bool isFeasible(const std::vector<chars>& str) {
    const std::size_t n = str.size();
    std::vector<int> isfeasible(3 * n * n);
    for (std::size_t i = 0; i < n; ++i) {
        if (str[i] == chars::a) {
            isfeasible[n * n * i + n * i + 0] = 1;
        } else if (str[i] == chars::b) {
            isfeasible[n * n * i + n * i + 1] = 1;
        } else if (str[i] == chars::c) {
            isfeasible[n * n * i + n * i + 2] = 1;
        } else {
            assert(0);
        }
    }
    for (std::size_t l = 1; l < n; ++l) {
        for (std::size_t i = 0; i < n - l; ++i) {
            std::size_t j = i + l;
            bool ka = false;
            bool kb = false;
            bool kc = false;
            for (std::size_t k = i; k < j; ++k) {
                bool lka = isfeasible[n * n * i + n * k + 0];
                bool lkb = isfeasible[n * n * i + n * k + 1];
                bool lkc = isfeasible[n * n * i + n * k + 2];
                bool rka = isfeasible[n * n * (k + 1) + n * j + 0];
                bool rkb = isfeasible[n * n * (k + 1) + n * j + 1];
                bool rkc = isfeasible[n * n * (k + 1) + n * j + 2];
                if (!ka && (lka && rkc) || (lkb && rkc) || (lkc && rka)) {
                    ka = true;
                }
                if (!kb && (lka && rka) || (lka && rkb) || (lkc && rkb)) {
                    kb = true;
                }
                if (!kc && (lkb && rka) || (lkc && rkb) || (lkc && rkc)) {
                    kc = true;
                }
            }
            isfeasible[n * n * i + n * j + 0] = ka;
            isfeasible[n * n * i + n * j + 1] = kb;
            isfeasible[n * n * i + n * j + 2] = kc;
        }
    }
    return isfeasible[n * n * 0 + n * (n - 1) + 0];
}

int main() {
    std::cout << isFeasible({chars::b, chars::b, chars::b, chars::b, chars::a, chars::c});

}
