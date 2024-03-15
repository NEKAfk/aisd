#include "../../include/dec_tree.h"

Node_dt::Node_dt(int64_t x, int32_t y) : x(x), y(y) {}

void dec_tree::tree2png(Node_dt* root, const char* png_file_name) {
  if (root == nullptr) {return;}

  FILE *dot_file = fopen("tree.dot", "wb");
  if (dot_file == nullptr) {return;}

  std::fprintf(dot_file, "digraph\n"
                         "{\n"
                         "bgcolor = \"grey\";\n"
                         "ranksep = \"equally\";\n"
                         "node[shape = \"Mrecord\"; style = \"filled\"; fillcolor = \"#58CD36\"];\n"
                         "{rank = source;");

  std::fprintf(dot_file, "nodel[label = \"<root> root: %p\"; fillcolor = \"lightblue\"];", root);

  std::fprintf(dot_file, "node%p[label = \"{<data> data: x = %ld, y = %ld | {<left> l: %p| <right> r: %p}}\"; fillcolor = \"orchid\"]};\n",
               root, root->x, root->y, root->left, root->right);

  subtree2png(root, root->left, "left", dot_file);
  subtree2png(root, root->right, "right", dot_file);

  std::fprintf(dot_file, "}\n");

  std::fclose(dot_file);

  std::array<char, MAX_STR_LEN> sys_cmd{};
  std::snprintf(sys_cmd.data(), sys_cmd.size() - 1, "dot tree.dot -T png -o %s", png_file_name);
  std::system(sys_cmd.data());
  std::fprintf(stdout, "Output to the %s\n", png_file_name);
}

std::mt19937 dec_tree::mt((std::random_device())());

void dec_tree::subtree2png(Node_dt* parent, Node_dt* child, const char* direction, FILE *dot_file)
{
  if(child == nullptr) return;

  fprintf(dot_file, "node%p[label = \"{<data> data: x = %ld, y = %ld | {<left> l: %p| <right> r: %p}}\"];\n",
          child, child->x, child->y, child->left, child->right);

  fprintf(dot_file, "node%p:<%s>:s -> node%p:<data>:n;\n", parent, direction, child);

  subtree2png(child, child->left, "left", dot_file);
  subtree2png(child, child->right, "right", dot_file);
}

dec_tree::dec_tree(std::vector<int64_t>& vec) {
  std::sort(vec.begin(), vec.end());
  std::queue<Node_dt*> queue;
  for (auto& x : vec) {
    queue.push(new Node_dt(x, static_cast<int32_t>(dec_tree::mt())));
  }
  while (queue.size() != 1) {
    if (queue.size() & 1U) {
      queue.push(queue.front()); queue.pop();
    }

    std::size_t prev_size = queue.size();
    for (size_t i = 0; i < prev_size / 2; i++) {
      Node_dt* p = queue.front(); queue.pop();
      Node_dt* q = queue.front(); queue.pop();
      queue.push(merge(p, q));
    }
  }
  root = queue.front();
}

dec_tree::dec_tree(Node_dt* node) : root(node) {}

std::pair<Node_dt*, Node_dt*> dec_tree::split(Node_dt* p, int64_t x) {
  if (p == nullptr) {
    return {nullptr, nullptr};
  }
  if (p->x <= x) {
    auto [left, right] = split(p->right, x);
    p->right = left;
    return {p, right};
  } else {
    auto [left, right] = split(p->left, x);
    p->left = right;
    return {left, p};
  }
}

Node_dt* dec_tree::merge(Node_dt* p, Node_dt* q) {
  if (p == nullptr) {return q;}
  if (q == nullptr) {return p;}
  if (p->y >= q->y) {
    p->right = merge(p->right, q);
    return p;
  } else {
    q->left = merge(p, q->left);
    return q;
  }
}

Node_dt* dec_tree::find(int64_t x) {
  Node_dt* start = root;
  while (start != nullptr && start->x != x) {
    if (start->x > x) {
      start = start->left;
    } else {
      start = start->right;
    }
  }
  return start;
}

void dec_tree::remove(int64_t x) {
  auto [p, q] = split(root, x);
  auto [pl, pr] = split(p, x - 1);
  root = merge(pl, q);
}

void dec_tree::insert(int64_t x) {
  Node_dt* node = new Node_dt(x, static_cast<int32_t>(dec_tree::mt()));
  auto [p, q] = split(root, node->x);
  p = merge(p, node);
  root = merge(p, q);
}

void dec_tree::draw(const char* png_file_name) {
  tree2png(root, png_file_name);
}