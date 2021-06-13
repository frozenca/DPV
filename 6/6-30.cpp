#include <memory>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <limits>
#include <iostream>

static constexpr std::size_t num_chars = 4;
static constexpr std::size_t len_seq = 4;
static constexpr double score_inf = std::numeric_limits<double>::max() / 3.0;

struct Node {
    std::array<char, len_seq> seq = {0};
    Node* parent = nullptr;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

};

static const std::string charset = "ACGT";

void reconstructMaxParsimony(std::unordered_set<Node*>& leaves) {
    std::unordered_map<Node*, std::array<std::array<double, num_chars>, len_seq>> S;

    for (auto leaf : leaves) {
        for (std::size_t i = 0; i < len_seq; ++i) {
            S[leaf][i] = {score_inf, score_inf, score_inf, score_inf};
            auto c = leaf->seq[i];
            if (c == 'A') {
                S[leaf][i][0] = 0.0;
            } else if (c == 'C') {
                S[leaf][i][1] = 0.0;
            } else if (c == 'G') {
                S[leaf][i][2] = 0.0;
            } else if (c == 'T') {
                S[leaf][i][3] = 0.0;
            } else {
                assert(0);
            }
        }
    }

    while (!leaves.empty()) {
        auto leaf = *leaves.begin();
        auto par = leaf->parent;
        assert(par);
        auto sibling = (leaf == par->left.get()) ? par->right.get() : par->left.get();

        for (std::size_t i = 0; i < len_seq; ++i) {
            auto& leaf_i = S[leaf][i];
            auto& sibling_i = S[sibling][i];
            std::array<double, num_chars> min_from_leaf = leaf_i;
            for (std::size_t c = 0; c < num_chars; ++c) {
                for (std::size_t d = 0; d < num_chars; ++d) {
                    if (c == d) {
                        min_from_leaf[c] = std::min(min_from_leaf[c], leaf_i[c]);
                    } else {
                        min_from_leaf[c] = std::min(min_from_leaf[c], leaf_i[d] + 1.0);
                    }
                }
            }
            std::array<double, num_chars> min_from_sibling = sibling_i;
            for (std::size_t c = 0; c < num_chars; ++c) {
                for (std::size_t d = 0; d < num_chars; ++d) {
                    if (c == d) {
                        min_from_sibling[c] = std::min(min_from_sibling[c], sibling_i[c]);
                    } else {
                        min_from_sibling[c] = std::min(min_from_sibling[c], sibling_i[d] + 1.0);
                    }
                }
            }
            std::array<double, num_chars> total_min = {0};
            auto min_i = min_from_leaf[0] + min_from_sibling[0];
            char min_char = 'A';
            for (std::size_t c = 0; c < num_chars; ++c) {
                total_min[c] = min_from_leaf[c] + min_from_sibling[c];
                if (min_i > total_min[c]) {
                    min_i = total_min[c];
                    min_char = charset[c];
                }
            }
            par->seq[i] = min_char;
            S[par][i] = total_min;
        }
        leaves.erase(leaf);
        leaves.erase(sibling);
        if (par->parent) {
            leaves.insert(par);
        }
    }
}

void print(const Node* node) {
    if (node) {
        for (auto ch : node->seq) {
            std::cout << ch;
        }
        std::cout << '\n';
    }
}

int main() {
    std::unordered_set<Node*> leaves;

    std::unique_ptr<Node> root = std::make_unique<Node>();
    root->left = std::make_unique<Node>();
    root->left->parent = root.get();
    root->right = std::make_unique<Node>();
    root->right->parent = root.get();
    root->left->left = std::make_unique<Node>();
    root->left->left->parent = root->left.get();
    root->left->right = std::make_unique<Node>();
    root->left->right->parent = root->left.get();
    root->left->right->seq = {'C', 'G', 'C', 'G'};
    leaves.insert(root->left->right.get());
    root->left->left->left = std::make_unique<Node>();
    root->left->left->left->parent = root->left->left.get();
    root->left->left->left->seq = {'A', 'T', 'T', 'C'};
    leaves.insert(root->left->left->left.get());
    root->left->left->right = std::make_unique<Node>();
    root->left->left->right->parent = root->left->left.get();
    root->left->left->right->seq = {'A', 'G', 'T', 'C'};
    leaves.insert(root->left->left->right.get());
    root->right->left = std::make_unique<Node>();
    root->right->left->parent = root->right.get();
    root->right->left->seq = {'A', 'G', 'G', 'A'};
    leaves.insert(root->right->left.get());
    root->right->right = std::make_unique<Node>();
    root->right->right->parent = root->right.get();
    root->right->right->seq = {'A', 'T', 'C', 'A'};
    leaves.insert(root->right->right.get());
    reconstructMaxParsimony(leaves);

    print(root.get());
    print(root->left.get());
    print(root->right.get());
    print(root->left->left.get());

}