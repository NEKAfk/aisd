#ifndef AISD_DEC_TREE_H
#define AISD_DEC_TREE_H
#include "log.h"

#include <queue>

#include <algorithm>
#include <array>
#include <ctime>
#include <random>
#include <vector>

struct Node_dt {
  int64_t x, y;

  Node_dt(int64_t x, int32_t y);

  Node_dt* left{};
  Node_dt* right{};
};

class dec_tree {
private:
  static const size_t MAX_STR_LEN = 100;

  static std::mt19937 mt;

  Node_dt* root{};

  static void tree2png(Node_dt* root, const char* png_file_name);

  static void subtree2png(Node_dt* parent, Node_dt* child, const char* direction, FILE *dot_file);

public:
  explicit dec_tree(std::vector<int64_t>& vec);

  explicit dec_tree(Node_dt* node);

  static std::pair<Node_dt*, Node_dt*> split(Node_dt* p, int64_t x);

  static Node_dt* merge(Node_dt* p, Node_dt* q);

  Node_dt* find(int64_t x);

  void remove(int64_t x);

  void insert(int64_t x);

  void draw(const char* png_file_name);
};
#endif // AISD_DEC_TREE_H