#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

void computeSentences(const std::string& s, std::size_t i, std::unordered_map<std::size_t, std::vector<std::string>>& memo,
                      const std::unordered_set<std::string>& wordDict) {
    if (!memo.count(i)) {
        for (std::size_t j = i + 1; j <= s.size(); j++) {
            auto word = s.substr(i, j - i);
            if (wordDict.count(word)) {
                computeSentences(s, j, memo, wordDict);
                for (const auto& tail : memo[j]) {
                    std::string new_tail = (tail.empty() ? "" : ' ' + tail);
                    auto new_word = word;
                    new_word.append(new_tail);
                    memo[i].push_back(new_word);
                }
            }
        }
    }
}

std::vector<std::string> wordBreak(const std::string& s,
                                   const std::unordered_set<std::string>& wordDict) {
    std::unordered_map<std::size_t, std::vector<std::string>> memo {{s.size(), {""}}};
    computeSentences(s, 0, memo, wordDict);
    return memo[0];
}

int main() {
    auto result = wordBreak("catsanddog", {"cat", "cats", "and", "sand", "dog"});
    for (const auto& res : result) {
        std::cout << res << '\n';
    }

}
