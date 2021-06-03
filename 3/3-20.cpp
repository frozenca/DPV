#include <cassert>
#include <cstddef>
#include <optional>
#include <memory>
#include <vector>

struct Node {
    std::size_t label;
    std::vector<std::unique_ptr<Node>> children;
    Node* parent = nullptr;
    Node(std::size_t label) : label {label} {}
};

void updateLabelHelper(Node* curr, std::vector<std::size_t>& labels) {
    if (!curr) {
        return;
    }
    labels.push_back(curr->label);
    curr->label = labels[std::max(0, static_cast<int>(labels.size() - 1) - static_cast<int>(curr->label))];
    for (auto&& child : curr->children) {
        updateLabelHelper(child.get(), labels);
    }
    labels.pop_back();
}

void updateLabel(Node* root) {
    std::vector<std::size_t> labels;
    updateLabelHelper(root,  labels);
}



int main() {

}
