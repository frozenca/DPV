#include <cassert>
#include <concepts>
#include <optional>
#include <memory>

template <std::regular T>
struct BTNode {
    T val;
    T max_val;
    std::unique_ptr<BTNode> left;
    std::unique_ptr<BTNode> right;
    BTNode(T val) : val{std::move(val)}, max_val {val} {}
};

template <std::regular T>
std::optional<T> computeMaxVal(BTNode<T>* root) {
    if (!root) {
        return {};
    }
    auto left_val = computeMaxVal(root->left.get());
    auto right_val = computeMaxVal(root->right.get());
    if (left_val.has_value()) {
        root->max_val = std::max(root->max_val, *left_val);
    }
    if (right_val.has_value()) {
        root->max_val = std::max(root->max_val, *right_val);
    }
    return root->max_val;
}

int main() {

}
