#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

int maxScore(const std::vector<int>& cardPoints, std::size_t k) {
    int rearScore = 0;
    const std::size_t n = cardPoints.size();

    int frontScore = std::accumulate(cardPoints.begin(), cardPoints.begin() + k, 0);
    // take all k cards from the beginning
    int maxScore = frontScore;

    // take i from the beginning and k - i from the end
    for (std::size_t i = k - 1; i < k; --i) {
        rearScore += cardPoints[n - (k - i)];
        frontScore -= cardPoints[i];
        int currentScore = rearScore + frontScore;
        maxScore = std::max(maxScore, currentScore);
    }

    return maxScore;
}
int main() {


}