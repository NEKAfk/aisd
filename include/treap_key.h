#ifndef AISD_TREAP_KEY_H
#define AISD_TREAP_KEY_H

#include <algorithm>
#include <array>
#include <ctime>
#include <queue>
#include <random>
#include <utility>
#include <vector>

class treap_key {
private:
  struct op {
    static const uint32_t NOP = 0;
    static const uint32_t ADD = 1;
    static const uint32_t SET = 2;

    uint32_t type;
    int64_t value;

    void clear() {
      type = op::NOP;
      value = 0;
    }

    op& operator+=(op const &other) {
      if (other.type == op::SET) {
        clear();
      }
      type = std::max(type, other.type);
      value += other.value;
      return *this;
    }
  };

  struct Node {
    int64_t size{1}, key, result, priority;
    op oper{op::NOP};
    bool reverse{false};

    explicit Node(int64_t key);

    Node* parent{};
    Node* left{};
    Node* right{};

    static std::pair<Node*, Node*> split(Node* p, int64_t idx);
    static Node* merge(Node* p, Node* q);
    static void update(Node* node);

    static void push(Node* node);
    static void apply(Node* node);
    static int64_t get_size(Node* node);
    static int64_t true_val(Node* node);

    static void clear(Node* root);
    ~Node() = default;
  };

  static const size_t MAX_STR_LEN = 100;
  static std::mt19937 mt;
  Node* root{};

  explicit treap_key(Node* node);

  static void modify(treap_key& tree, int64_t l, int64_t r, op oper, bool reverse);

  static void tree2png(Node* root, const char* png_file_name);

  static void subtree2png(Node* parent, Node* child, const char* direction, FILE *dot_file);

public:
  treap_key() = default;
  explicit treap_key(std::vector<int64_t>& vec);
  treap_key(const treap_key& other) = delete;
  treap_key& operator=(const treap_key& other) = delete;
  treap_key(treap_key&& other) noexcept;

  static void split(treap_key& tree, int64_t idx, treap_key& l, treap_key& r);
  static void merge(treap_key& fst, treap_key& snd, treap_key& dst);

  void add(int64_t l, int64_t r, int64_t x);
  void set(int64_t l, int64_t r, int64_t x);
  void reverse(int64_t l, int64_t r);
  int64_t sum(int64_t l, int64_t r);

  ~treap_key();

  static void draw(const treap_key& tree, const char* png_file_name);
};
#endif // AISD_TREAP_KEY_H