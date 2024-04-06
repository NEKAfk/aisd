#ifndef AISD_AVL_TREE_H
#define AISD_AVL_TREE_H

#include <cstdint>
#include <algorithm>

class avl_tree {
private:
  struct Node {
    Node(int64_t key);
    int64_t key, size{1}, height{1};
    Node* left{};
    Node* right{};

    static Node* balance_node(Node* node);

    static Node* left_rotate(Node* node);

    static Node* right_rotate(Node* node);

    static int64_t get_balance(Node* node);

    static void update(Node* node);

    static int64_t get_height(Node* node);

    static int64_t get_size(Node* node);

    ~Node();
  };

  Node* root{};

  static Node* insert_impl(Node* root, int64_t x);

  static std::pair<Node*, Node*> get_min(Node* root);

  static Node* remove_impl(Node* root, int64_t x);

public:
  void insert(int64_t x);

  void remove(int64_t x);

  ~avl_tree();
};

#endif // AISD_AVL_TREE_H
