#include <cassert>
#include <iostream>
#include <memory>

template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left(std::move(left))
        , right(std::move(right)) {
    }

    T value;
    TreeNodePtr<T> left;
    TreeNodePtr<T> right;
    TreeNode* parent = nullptr;
};

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) noexcept {
    if (node->left.get()) {
        if (node->left.get()->value > node->value || !CheckTreeProperty(node->left.get())) {
            return false;
        }
    }

    if (node->right.get()) {
        if(node->right.get()->value < node->value|| !CheckTreeProperty(node->left.get()))
            return false;
    }
    return true;
}

template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept {
    while (node->left.get()) {
        node = node->left.get();
    }
    // Реализуйте функцию
    return node;
}

template <typename T>
TreeNode<T>* next(TreeNode<T>* node) noexcept {
    if (node->right.get()) {
        node = node->right.get();
        while (node->left) {
            node = node->left.get();
        }
    }
    else {
        if (!node->parent) return nullptr;
        while (node->parent) {
            if (node->parent->right.get() == node) {
                node = node->parent;
                if (node->parent == nullptr) return nullptr;
            }
            return node->parent;
        }
    }

    return node;
    // Реализуйте функцию
}

// функция создаёт новый узел с заданным значением и потомками
TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {}) {
  //  if (left.get()->parent != nullptr || right.get()->parent != nullptr) throw std::invalid_argument("need to be without parent");

    TreeNodePtr<int> ptr = std::make_unique<TreeNode<int>>(std::move(val), std::move(left), std::move(right));
    if (ptr->left) {
        ptr->left->parent = ptr.get();
    }
    if (ptr->right) {
        ptr->right->parent = ptr.get();
    }
        // Реализуйте функцию
    return ptr;
}   

//int main() {
//    using namespace std;
//    using T = TreeNode<int>;
//    auto root1 = N(6, N(4, N(3), N(5)), N(7));
//    assert(CheckTreeProperty(root1.get()));
//
//    T* iter = begin(root1.get());
//    while (iter) {
//        cout << iter->value << " "s;
//        iter = next(iter);
//    }
//    cout << endl;
//
//    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
//    assert(!CheckTreeProperty(root2.get()));
//
//    // Функция DeleteTree не нужна. Узлы дерева будут рекурсивно удалены
//    // благодаря деструкторам unique_ptr
//}