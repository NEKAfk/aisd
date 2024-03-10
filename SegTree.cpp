#include <cstdint>
#include <vector>

class segment_tree {
private:
  struct op {
    static uint32_t const NOP = 0;
    static uint32_t const ADD = 1;
    static uint32_t const SET = 2;

    uint32_t type;
    int64_t value;

    void reset() {
      type = op::NOP;
      value = 0;
    }

    op operator+=(op const &other) {
      if (other.type == op::SET) {
        reset();
      }
      type = std::max(type, other.type);
      value += other.value;
      return *this;
    }
  };

  struct vertex {
    std::size_t l{}, r{};
    int64_t result = 0;
    op mods{0, 0};

    std::size_t len() const {
      return r - l;
    }

    int64_t true_sum() const {
      int64_t orig = mods.type == op::SET ? 0LL : result;
      int64_t mod = mods.type ? mods.value * static_cast<int64_t>(len()) : 0LL;
      return orig + mod;
    }
  };

  std::vector<vertex> tree;

  void recalc(std::size_t idx) {
    tree[idx].result = tree[2 * idx + 1].true_sum() + tree[2 * idx + 2].true_sum();
  }

  void build(std::size_t idx, std::size_t l, std::size_t r) {
    tree[idx].l = l;
    tree[idx].r = r;
    if (tree[idx].len() == 1) {
      return;
    }
    std::size_t m = l + (r - l) / 2;
    build(2 * idx + 1, l, m);
    build(2 * idx + 2, m, r);
  }

  void push(std::size_t idx) {
    tree[2 * idx + 1].mods += tree[idx].mods;
    tree[2 * idx + 2].mods += tree[idx].mods;
    recalc(idx);
    tree[idx].mods.reset();
  }

  int64_t sum(std::size_t idx, std::size_t l, std::size_t r) {
    if (tree[idx].r <= l || r <= tree[idx].l) {
      return 0;
    }
    if (l <= tree[idx].l && tree[idx].r <= r) {
      return tree[idx].true_sum();
    }
    push(idx);
    return sum(2 * idx + 1, l, r) + sum(2 * idx + 2, l, r);
  }

  void modify(std::size_t idx, std::size_t l, std::size_t r,
              uint32_t op_type, int64_t value) {
    if (tree[idx].r <= l || r <= tree[idx].l) {
      return;
    }
    if (l <= tree[idx].l && tree[idx].r <= r) {
      tree[idx].mods += op{op_type, value};
    } else {
      push(idx);
      modify(2 * idx + 1, l, r, op_type, value);
      modify(2 * idx + 2, l, r, op_type, value);
      recalc(idx);
    }
  }

public:
  explicit segment_tree(std::size_t n) : tree(4 * n) {
    build(0, 0, n);
  }

  int64_t sum(std::size_t l, std::size_t r) {
    return sum(0, l, r);
  }

  void set_value(std::size_t l, std::size_t r, int64_t value) {
    modify(0, l, r, op::SET, value);
  }

  void add(std::size_t l, std::size_t r, int64_t value) {
    modify(0, l, r, op::ADD, value);
  }
};