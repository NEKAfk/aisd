#ifndef AISD_SEG_TREE_H
#define AISD_SEG_TREE_H
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

class seg_tree {
private:
  static const std::size_t MAX_STR_LEN = 1000;

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

    op operator+=(op const &other) {
      if (other.type == op::SET) {
        clear();
      }
      type = std::max(type, other.type);
      value += other.value;
      return *this;
    }
  };

  struct Node {
    std::size_t l{}, r{};
    int64_t result = 0;
    op mods{0, 0};

    std::size_t len() const;

    int64_t modified_result() const;
  };

  std::vector<Node> tree;

  void recalc(std::size_t idx);

  void build(std::size_t idx, std::size_t l, std::size_t r);

  void push(std::size_t idx);

  int64_t sum(std::size_t idx, std::size_t l, std::size_t r);

  void modify(std::size_t idx, std::size_t l, std::size_t r, op mod);

  void tree2png(const char* png_file_name, std::size_t idx);

  void subtree2png(std::size_t parent, std::size_t child, const char* direction, FILE *dot_file);

public:
  explicit seg_tree(std::size_t n);

  int64_t sum(std::size_t l, std::size_t r);

  void set_value(std::size_t l, std::size_t r, int64_t value);

  void add(std::size_t l, std::size_t r, int64_t value);

  void draw(const char* png_file_name);
};
#endif // AISD_SEG_TREE_H