#include "../../include/treap_key.h"

std::mt19937 treap_key::mt((std::random_device())());

// Node

treap_key::Node::Node(int64_t key) : key(key), result(key), priority(static_cast<int32_t>(mt())) {}

std::pair<treap_key::Node*, treap_key::Node*> treap_key::Node::split(Node* p, int64_t idx) {
  if (!p) {
    return {nullptr, nullptr};
  }
  Node::push(p);
  if (Node::get_size(p->left) < idx) {
    auto [left, right] = split(p->right, idx - Node::get_size(p->left) - 1);
    p->right = left;
    if (left) left->parent = p;
    if (right) right->parent = nullptr;
    Node::update(p);
    return {p, right};
  } else {
    auto [left, right] = split(p->left, idx);
    p->left = right;
    if (left) left->parent = nullptr;
    if (right) right->parent = p;
    Node::update(p);
    return {left, p};
  }
}
treap_key::Node* treap_key::Node::merge(Node* p, Node* q) {
  if (!p) {
    return q;
  } else if (!q) {
    return p;
  }
  Node::push(p);
  Node::push(q);
  if (p->priority >= q->priority) {
    p->right = merge(p->right, q);
    p->right->parent = p;
    Node::update(p);
    return p;
  } else {
    q->left = merge(p, q->left);
    q->left->parent = q;
    Node::update(q);
    return q;
  }
}
void treap_key::Node::update(Node* node) {
  if (!node) {
    return;
  }
  node->size = get_size(node->left) + 1 + get_size(node->right);
  node->result = true_val(node->left) + true_val(node->right) + node->key;
}
int64_t treap_key::Node::get_size(Node* node) {
  return node ? node->size : 0;
}

void treap_key::Node::apply(Node* node) {
  if (!node) return;
  switch (node->oper.type) {
    case (op::ADD):
      node->key += node->oper.value;
      break;
    case (op::SET):
      node->key = node->oper.value;
      break;
    default:
      break;
  }
  node->oper.clear();
}
void treap_key::Node::push(Node* node) {
  if (!node) return;
  if (node->left) node->left->oper += node->oper;
  if (node->right) node->right->oper += node->oper;
  Node::apply(node);
  if (node->reverse) {
    std::swap(node->left, node->right);
    node->reverse = false;
    if (node->left) node->left->reverse = !node->left->reverse;
    if (node->right) node->right->reverse = !node->right->reverse;
  }
  Node::update(node);
}
int64_t treap_key::Node::true_val(Node* node) {
  if (!node) return 0;
  int64_t prev = node->oper.type == op::SET ? 0 : node->result;
  int64_t mod = node->oper.type ? node->oper.value * Node::get_size(node) : 0;
  return prev + mod;
}

void treap_key::Node::clear(Node* root) {
  if (root == nullptr) {
    return;
  }
  Node* p = root;
  while (true) {
    while (p->left != nullptr || p->right != nullptr) {
      if (p->left) {
        p = p->left;
      } else {
        p = p->right;
      }
    }
    if (p == root) {
      delete root;
      return;
    }
    Node* t = p;
    p = p->parent;
    if (p->left == t) {
      p->left = nullptr;
    } else {
      p->right = nullptr;
    }
    delete t;
  }
}

//TREAP

treap_key::treap_key(std::vector<int64_t>& vec) {
  std::queue<Node*> queue;
  for (auto& x : vec) {
    queue.push(new Node(x));
  }
  while (queue.size() != 1) {
    if (queue.size() & 1U) {
      queue.push(queue.front());
      queue.pop();
    }

    std::size_t prev_size = queue.size();
    for (size_t i = 0; i < prev_size / 2; i++) {
      Node* p = queue.front();
      queue.pop();
      Node* q = queue.front();
      queue.pop();
      queue.push(Node::merge(p, q));
    }
  }
  root = queue.front();
}

treap_key::treap_key(Node* node) : root(node) {
  root->parent = nullptr;
}

treap_key::treap_key(treap_key&& other) noexcept : root(std::exchange(other.root, nullptr)) {
  std::fprintf(stdout, "Ctror\n");
}

void treap_key::split(treap_key& tree, int64_t idx, treap_key& l, treap_key& r) {
  auto [left, right] = Node::split(tree.root, idx);
  tree.root = nullptr;
  Node::clear(l.root);
  l.root = left;
  Node::clear(r.root);
  r.root = right;
}

void treap_key::merge(treap_key& fst, treap_key& snd, treap_key& dst) {
  auto p = Node::merge(fst.root, snd.root);
  fst.root = nullptr;
  snd.root = nullptr;
  Node::clear(dst.root);
  dst.root = p;
}

void treap_key::modify(treap_key& tree, int64_t l, int64_t r, op oper, bool reverse) {
  auto [p, q] = Node::split(tree.root, l);
  auto [ql, qr] = Node::split(q, r - l);
  Node::push(ql);
  if (ql && reverse) {
    ql->reverse = true;
  }
  if (ql) ql->oper = oper;
  tree.root = (Node::merge(Node::merge(p, ql), qr));
}

void treap_key::add(int64_t l, int64_t r, int64_t x) {
  modify(*this, l, r, {op::ADD, x}, false);
}

void treap_key::set(int64_t l, int64_t r, int64_t x) {
  modify(*this, l, r, {op::SET, x}, false);
}

void treap_key::reverse(int64_t l, int64_t r) {
  modify(*this, l, r, {op::NOP}, true);
}

int64_t treap_key::sum(int64_t l, int64_t r) {
  auto [p, q] = Node::split(root, l);
  auto [ql, qr] = Node::split(q, r - l);
  Node::push(ql);
  int64_t result = ql->result;
  root = (Node::merge(Node::merge(p, ql), qr));
  return result;
}

treap_key::~treap_key() {
  Node::clear(root);
}

// Draw

void treap_key::tree2png(Node* root, const char* png_file_name) {
  if (!root) return;
  Node::push(root);

  FILE* dot_file = fopen("tree.dot", "wb");
  if (dot_file == nullptr) {
    return;
  }

  std::fprintf(dot_file, "digraph\n"
                         "{\n"
                         "bgcolor = \"grey\";\n"
                         "ranksep = \"equally\";\n"
                         "node[shape = \"Mrecord\"; style = \"filled\"; fillcolor = \"#58CD36\"];\n"
                         "{rank = source;");

  std::fprintf(dot_file, "nodel[label = \"<root> root: %p\"; fillcolor = \"lightblue\"];", root);

  std::fprintf(dot_file,
               "node%p[label = \"{<data> data: key = %ld, priority = %ld | {<left> l: %p| <right> r: %p}}\"; fillcolor = "
               "\"orchid\"]};\n",
               root, root->key, root->priority, root->left, root->right);

  subtree2png(root, root->left, "left", dot_file);
  subtree2png(root, root->right, "right", dot_file);

  std::fprintf(dot_file, "}\n");

  std::fclose(dot_file);

  std::array<char, MAX_STR_LEN> sys_cmd{};
  std::snprintf(sys_cmd.data(), sys_cmd.size() - 1, "dot tree.dot -T png -o %s", png_file_name);
  std::system(sys_cmd.data());
  std::fprintf(stdout, "Output to the %s\n", png_file_name);
}

void treap_key::subtree2png(treap_key::Node* parent, treap_key::Node* child, const char* direction, FILE* dot_file) {
  if (!child) return;
  Node::push(child);

  fprintf(dot_file, "node%p[label = \"{<data> data: key = %ld, priority = %ld | {<left> l: %p| <right> r: %p}}\"];\n", child,
          child->key, child->priority, child->left, child->right);

  fprintf(dot_file, "node%p:<%s>:s -> node%p:<data>:n;\n", parent, direction, child);

  subtree2png(child, child->left, "left", dot_file);
  subtree2png(child, child->right, "right", dot_file);
}

void treap_key::draw(const treap_key& tree, const char* png_file_name) {
  tree2png(tree.root, png_file_name);
}
