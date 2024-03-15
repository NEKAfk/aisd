#ifndef AISD_PERS_SEG_TREE_H
#define AISD_PERS_SEG_TREE_H
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

class pers_seg_tree {
private:
  static const std::size_t MAX_STR_LEN = 100;

  struct op {
    static const uint32_t ADD = 1;
    static const uint32_t SET = 2;

    uint32_t type;
    int64_t value;

    void apply(int64_t& result) const;
  };

  struct Node {
    std::size_t l{}, r{};
    int64_t result = 0;
    Node* left{};
    Node* right{};

    std::size_t len() const;

    int64_t sum() const;
  };

  std::vector<Node*> roots;

  static void recalc(Node* node);

  void build(Node* node, std::size_t l, std::size_t r);

  int64_t sum(Node* node, std::size_t l, std::size_t r);

  Node* modify(Node* node, std::size_t l, std::size_t r,
              uint32_t op_type, int64_t value);

public:
  explicit pers_seg_tree(std::size_t n);

  int64_t sum(std::size_t l, std::size_t r);

  void set_value(std::size_t idx, int64_t value);

  void add(std::size_t idx, int64_t value);

  void draw();

  static void tree2png(const char* png_file_name, Node* root);

  static void subtree2png(Node* parent, Node* child, const char* direction, FILE *dot_file);
};
#endif // AISD_PERS_SEG_TREE_H
