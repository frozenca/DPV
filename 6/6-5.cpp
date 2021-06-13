#include <algorithm>
#include <array>
#include <cassert>
#include <vector>
#include <random>
#include <iostream>

int getMaxScore(const std::vector<std::array<int, 4>>& checkerboard) {
    const std::size_t n = checkerboard.size();
    std::vector<std::array<int, 8>> scores (n);
    // 0 : [. . . .]
    // 1 : [O . . .]
    // 2 : [. O . .]
    // 3 : [. . O .]
    // 4 : [. . . O]
    // 5 : [O . O .]
    // 6 : [O . . O]
    // 7 : [. O . O]
    // 0 <- 0, 1, 2, 3, 4, 5, 6, 7
    // 1 <- 0, 2, 3, 4, 7
    // 2 <- 0, 1, 3, 4, 5, 6
    // 3 <- 0, 1, 2, 4, 6, 7
    // 4 <- 0, 1, 2, 3, 5
    // 5 <- 0, 2, 4, 7
    // 6 <- 0, 2, 3
    // 7 <- 0, 1, 3, 5
    assert(n);
    scores[0][1] = checkerboard[0][0];
    scores[0][2] = checkerboard[0][1];
    scores[0][3] = checkerboard[0][2];
    scores[0][4] = checkerboard[0][3];
    scores[0][5] = checkerboard[0][0] + checkerboard[0][2];
    scores[0][6] = checkerboard[0][0] + checkerboard[0][3];
    scores[0][7] = checkerboard[0][1] + checkerboard[0][3];
    for (std::size_t i = 1; i < n; ++i) {
        scores[i][0] = std::max({scores[i - 1][0], scores[i - 1][1], scores[i - 1][2], scores[i - 1][3],
                                 scores[i - 1][4], scores[i - 1][5], scores[i - 1][6], scores[i - 1][7]});
        scores[i][1] = std::max({scores[i - 1][0], scores[i - 1][2], scores[i - 1][3],
                                        scores[i - 1][4], scores[i - 1][7]});
        scores[i][2] = std::max({scores[i - 1][0], scores[i - 1][1], scores[i - 1][3],
                                 scores[i - 1][4], scores[i - 1][5], scores[i - 1][6]});
        scores[i][3] = std::max({scores[i - 1][0], scores[i - 1][1], scores[i - 1][2],
                                 scores[i - 1][4], scores[i - 1][6], scores[i - 1][7]});
        scores[i][4] = std::max({scores[i - 1][0], scores[i - 1][1], scores[i - 1][2],
                                 scores[i - 1][3], scores[i - 1][5]});
        scores[i][5] = std::max({scores[i - 1][0], scores[i - 1][2], scores[i - 1][4], scores[i - 1][7]});
        scores[i][6] = std::max({scores[i - 1][0], scores[i - 1][2], scores[i - 1][3]});
        scores[i][7] = std::max({scores[i - 1][0], scores[i - 1][1], scores[i - 1][3], scores[i - 1][5]});
    }
    return *std::ranges::max_element(scores.back());
}

int main() {
    constexpr std::size_t n = 20;
    std::vector<std::array<int, 4>> checkerboard (n);
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> score (-10, 100);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            checkerboard[i][j] = score(gen);
        }
    }
    std::cout << getMaxScore(checkerboard);





}
