#ifndef AISD_TREAP_H
#define AISD_TREAP_H
#include <queue>

#include <algorithm>
#include <array>
#include <ctime>
#include <random>
#include <vector>

class treap {
public:
  struct Node {
    int64_t key, priority;

    explicit Node(int64_t key);

    Node* parent{};
    Node* left{};
    Node* right{};

    static std::pair<Node*, Node*> split(Node* p, int64_t x);
    static Node* merge(Node* p, Node* q);

    static void clear(Node* root);

    ~Node() = default;
  };

private:
  static const size_t MAX_STR_LEN = 100;

  static std::mt19937 mt;

  Node* root{};

  static void tree2png(Node* root, const char* png_file_name);

  static void subtree2png(Node* parent, Node* child, const char* direction, FILE *dot_file);

public:
  explicit treap(std::vector<int64_t>& vec);

  bool contains(int64_t key);

  void remove(int64_t key);

  void insert(int64_t key);

  ~treap();

  static void draw(treap& tree, const char* png_file_name);
};
#endif // AISD_TREAP_H