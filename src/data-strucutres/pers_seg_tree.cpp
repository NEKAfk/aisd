#include "../../include/pers_seg_tree.h"

static const std::size_t MAX_STR_LEN = 100;

void pers_seg_tree::op::apply(int64_t& result) const {
  if (type == op::ADD) {
    result += value;
  } else if (type == op::SET) {
    result = value;
  }
}

std::size_t pers_seg_tree::Node::len() const {
  return r - l;
}

int64_t pers_seg_tree::Node::sum() const {
  return result;
}

void pers_seg_tree::recalc(Node* node) {
  node->result = node->left->sum() + node->right->sum();
}

void pers_seg_tree::build(Node* node, std::size_t l, std::size_t r) {
  node->l = l;
  node->r = r;
  if (node->len() == 1) {
    return;
  }
  node->left = new Node();
  node->right = new Node();
  std::size_t m = l + (r - l) / 2;
  build(node->left, l, m);
  build(node->right, m, r);
}

int64_t pers_seg_tree::sum(Node* node, std::size_t l, std::size_t r) {
  if (node->r <= l || r <= node->l) {
    return 0;
  }
  if (l <= node->l && node->r <= r) {
    return node->sum();
  }
  return sum(node->left, l, r) + sum(node->right, l, r);
}

pers_seg_tree::Node* pers_seg_tree::modify(Node* node, std::size_t l, std::size_t r,
             uint32_t op_type, int64_t value) {
  if (node->r <= l || r <= node->l) {
    return node;
  }
  Node* res = new Node(*node);
  if (l <= node->l && node->r <= r) {
    op{op_type, value}.apply(res->result);
  } else {
    res->left = modify(node->left, l, r, op_type, value);
    res->right = modify(node->right, l, r, op_type, value);
    recalc(res);
  }
  return res;
}

pers_seg_tree::pers_seg_tree(std::size_t n) : roots(1) {
  roots[0] = new Node();
  build(roots[0], 0, n);
}

int64_t pers_seg_tree::sum(std::size_t l, std::size_t r) {
  return sum(roots.back(), l, r);
}

void pers_seg_tree::set_value(std::size_t idx, int64_t value) {
  roots.push_back(modify(roots.back(), idx, idx + 1, op::SET, value));
}

void pers_seg_tree::add(std::size_t idx, int64_t value) {
  roots.push_back(modify(roots.back(), idx, idx + 1, op::ADD, value));
}

void pers_seg_tree::draw() {
  for (size_t i = 0; i < roots.size(); i++) {
    tree2png((std::to_string(i) + ".png").c_str(), roots[i]);
  }
}

void pers_seg_tree::tree2png(const char* png_file_name, Node* root) {
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

  std::fprintf(dot_file,
               "node%p[label = \"{<data> data: key = %ld | {<left> l: %p| <right> r: %p}}\"; fillcolor = \"orchid\"]};\n",
               root, root->result, root->left, root->right);

  subtree2png(root, root->left, "left", dot_file);
  subtree2png(root, root->right, "right", dot_file);

  std::fprintf(dot_file, "}\n");

  std::fclose(dot_file);

  std::array<char, MAX_STR_LEN> sys_cmd{};
  std::snprintf(sys_cmd.data(), sys_cmd.size() - 1, "dot tree.dot -T png -o %s", png_file_name);
  std::system(sys_cmd.data());
  std::fprintf(stdout, "Output to the %s\n", png_file_name);
}

void pers_seg_tree::subtree2png(Node* parent, Node* child, const char* direction, FILE *dot_file)
{
  if(child == nullptr) return;

  fprintf(dot_file, "node%p[label = \"{<data> data: key = %ld | {<left> l: %p| <right> r: %p}}\"];\n",
          child, child->result, child->left, child->right);

  fprintf(dot_file, "node%p:<%s>:s -> node%p:<data>:n;\n", parent, direction, child);

  subtree2png(child, child->left, "left", dot_file);
  subtree2png(child, child->right, "right", dot_file);
}
