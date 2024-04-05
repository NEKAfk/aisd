#include "../../include/seg_tree.h"

std::size_t seg_tree::Node::len() const {
  return r - l;
}

int64_t seg_tree::Node::modified_result() const {
  int64_t prev = mods.type == op::SET ? 0LL : result;
  int64_t mod = mods.type ? mods.value * static_cast<int64_t>(len()) : 0LL;
  return prev + mod;
}

void seg_tree::recalc(std::size_t idx) {
  tree[idx].result = tree[2 * idx + 1].modified_result() + tree[2 * idx + 2].modified_result();
}

void seg_tree::build(std::size_t idx, std::size_t l, std::size_t r) {
  tree[idx].l = l;
  tree[idx].r = r;
  if (tree[idx].len() == 1) {
    return;
  }
  std::size_t m = l + (r - l) / 2;
  build(2 * idx + 1, l, m);
  build(2 * idx + 2, m, r);
}

void seg_tree::push(std::size_t idx) {
  tree[2 * idx + 1].mods += tree[idx].mods;
  tree[2 * idx + 2].mods += tree[idx].mods;
  recalc(idx);
  tree[idx].mods.clear();
}

int64_t seg_tree::sum(std::size_t idx, std::size_t l, std::size_t r) {
  if (tree[idx].r <= l || r <= tree[idx].l) {
    return 0;
  }
  if (l <= tree[idx].l && tree[idx].r <= r) {
    return tree[idx].modified_result();
  }
  push(idx);
  return sum(2 * idx + 1, l, r) + sum(2 * idx + 2, l, r);
}

void seg_tree::modify(std::size_t idx, std::size_t l, std::size_t r,
            op mod) {
  if (tree[idx].r <= l || r <= tree[idx].l) {
    return;
  }
  if (l <= tree[idx].l && tree[idx].r <= r) {
    tree[idx].mods += mod;
  } else {
    push(idx);
    modify(2 * idx + 1, l, r, mod);
    modify(2 * idx + 2, l, r, mod);
    recalc(idx);
  }
}

seg_tree::seg_tree(std::size_t n) : tree(4 * n) {
  build(0, 0, n);
}

int64_t seg_tree::sum(std::size_t l, std::size_t r) {
  return sum(0, l, r);
}

void seg_tree::set_value(std::size_t l, std::size_t r, int64_t value) {
  modify(0, l, r, {op::SET, value});
}

void seg_tree::add(std::size_t l, std::size_t r, int64_t value) {
  modify(0, l, r, {op::ADD, value});
}

void seg_tree::draw(const char* png_file_name) {
  tree2png(png_file_name, 0);
}

void seg_tree::tree2png(const char* png_file_name, std::size_t idx) {
  FILE *dot_file = fopen("tree.dot", "wb");
  if (dot_file == nullptr) {return;}

  std::fprintf(dot_file, "digraph\n"
                         "{\n"
                         "bgcolor = \"grey\";\n"
                         "ranksep = \"equally\";\n"
                         "node[shape = \"Mrecord\"; style = \"filled\"; fillcolor = \"#58CD36\"];\n"
                         "{rank = source;");

  std::fprintf(dot_file, "nodel[label = \"<root> root: %ld\"; fillcolor = \"lightblue\"];", idx);

  std::fprintf(dot_file,
               "node%ld[label = \"{<data> data: key = %ld | {<left> l: %zu| <right> r: %zu}}\"; fillcolor = \"orchid\"]};\n",
               idx, tree[idx].modified_result(), tree[idx].l, tree[idx].r);

  if (tree[idx].len() > 1) {
    push(idx);
    subtree2png(idx, 2 * idx + 1, "left", dot_file);
    subtree2png(idx, 2 * idx + 2, "right", dot_file);
  }

  std::fprintf(dot_file, "}\n");

  std::fclose(dot_file);

  std::array<char, MAX_STR_LEN> sys_cmd{};
  std::snprintf(sys_cmd.data(), sys_cmd.size() - 1, "dot tree.dot -T png -o %s", png_file_name);
  std::system(sys_cmd.data());
  std::fprintf(stdout, "Output to the %s\n", png_file_name);
}

void seg_tree::subtree2png(std::size_t parent, std::size_t child, const char* direction, FILE *dot_file)
{
  fprintf(dot_file, "node%ld[label = \"{<data> data: key = %ld | {<left> l: %zu| <right> r: %zu}}\"];\n",
          child, tree[child].modified_result(), tree[child].l, tree[child].r);

  fprintf(dot_file, "node%ld:<%s>:s -> node%ld:<data>:n;\n", parent, direction, child);

  if (tree[child].len() > 1) {
    push(child);
    subtree2png(child, 2 * child + 1, "left", dot_file);
    subtree2png(child, 2 * child + 2, "right", dot_file);
  }
}