#include "../../include/avl_tree.h"

// Node

avl_tree::Node::Node(int64_t key) : key(key) {}

avl_tree::Node* avl_tree::Node::balance_node(Node* node) {
  if (!node) return node;
  int64_t balance = get_balance(node);
  if (balance == -2) {
    if (get_balance(node->right) == 1) {
      node->right = right_rotate(node->right);
    }
    return left_rotate(node);
  } else if (balance == 2) {
    if (get_balance(node->right) == -1) {
      node->left = left_rotate(node->left);
    }
    return right_rotate(node);
  }
  return node;
}

avl_tree::Node* avl_tree::Node::left_rotate(Node* node) {
  if (!node) return node;
  Node* right = node->right;
  node->right = right->left;
  update(node);
  right->left = node;
  update(right);
  return right;
}

avl_tree::Node* avl_tree::Node::right_rotate(Node* node) {
  if (!node) return node;
  Node* left = node->left;
  node->left = left->right;
  update(node);
  left->right = node;
  update(left);
  return left;
}

int64_t avl_tree::Node::get_balance(Node* node) {
  if (!node) return 0;
  return get_height(node->left) - get_height(node->right);
}

void avl_tree::Node::update(Node* node) {
  if (!node) return;
  node->size = get_size(node->left) + 1 + get_size(node->right);
  node->height = 1 + std::max(get_height(node->left), get_height(node->right));
}

int64_t avl_tree::Node::get_height(Node* node) {
  return node ? node->height : 0;
}

int64_t avl_tree::Node::get_size(Node* node) {
  return node ? node->size : 0;
}

avl_tree::Node::~Node() {
    delete left;
    delete right;
}

// Tree

avl_tree::Node* avl_tree::insert_impl(Node* root, int64_t x) {
  if (!root) return new Node{x};
  if (x <= root->key) {
    root->left = insert_impl(root->left, x);
  } else {
    root->right = insert_impl(root->right, x);
  }
  Node::update(root);
  root = Node::balance_node(root);
  return root;
}

std::pair<avl_tree::Node*, avl_tree::Node*> avl_tree::get_min(Node* root) {
  if (!root->left) {
    return {nullptr, root};
  }
  auto [left, new_root] = get_min(root->left);
  root->left = left;
  Node::update(root);
  root = Node::balance_node(root);
  return {root, new_root};
}

avl_tree::Node* avl_tree::remove_impl(Node* root, int64_t x) {
  if (!root) return root;
  if (x == root->key) {
    if (!root->right) {
      Node* left = root->left;
      root->left = nullptr;
      delete root;
      root = left;
    } else {
      Node* left = root->left;
      auto [right, new_root] = get_min(root->right);
      root->left = nullptr;
      root->right = nullptr;
      delete root;
      root = new_root;
      root->left=  left;
      root->right = right;
    }
  } else if (x <= root->key) {
    root->left = remove_impl(root->left, x);
  } else {
    root->right = remove_impl(root->right, x);
  }
  Node::update(root);
  root = Node::balance_node(root);
  return root;
}

void avl_tree::insert(int64_t x) {
  root = insert_impl(root, x);
}

void avl_tree::remove(int64_t x) {
  root = remove_impl(root, x);
}

avl_tree::~avl_tree() {
  delete root;
}
