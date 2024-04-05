#include "../../include/treap.h"

std::mt19937 treap::mt((std::random_device())());

// Node

treap::Node::Node(int64_t key) : key(key), priority(static_cast<int32_t>(mt())) {}

std::pair<treap::Node*, treap::Node*> treap::Node::split(Node* p, int64_t x) {
  if (p == nullptr) {
    return {nullptr, nullptr};
  }
  if (p->key <= x) {
    auto [left, right] = split(p->right, x);
    p->right = left;
    if (left) left->parent = p;
    if (right) right->parent = nullptr;
    return {p, right};
  } else {
    auto [left, right] = split(p->left, x);
    p->left = right;
    if (left) left->parent = nullptr;
    if (right) right->parent = p;
    return {left, p};
  }
}

treap::Node* treap::Node::merge(Node* p, Node* q) {
  if (p == nullptr) {
    return q;
  } else if (q == nullptr) {
    return p;
  }
  if (p->priority >= q->priority) {
    p->right = merge(p->right, q);
    p->right->parent = p;
    return p;
  } else {
    q->left = merge(p, q->left);
    q->left->parent = q;
    return q;
  }
}

void treap::Node::clear(Node* root) {
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

// Treap

treap::treap(std::vector<int64_t>& vec) {
  std::sort(vec.begin(), vec.end());
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

treap::~treap() {
  Node::clear(this->root);
}

bool treap::contains(int64_t key) {
  auto [left, right] = Node::split(root, key);
  auto [lleft, rleft] = Node::split(left, key - 1);
  Node* res = rleft;
  root = Node::merge(Node::merge(lleft, rleft), right);
  return res != nullptr;
}

void treap::remove(int64_t key) {
  auto [p, q] = Node::split(root, key);
  auto [pl, pr] = Node::split(p, key - 1);
  Node::clear(pr);
  root = Node::merge(pl, q);
}

void treap::insert(int64_t key) {
  Node* node = new Node(key);
  auto [p, q] = Node::split(root, node->key);
  root = Node::merge(Node::merge(p, node), q);
}

void treap::tree2png(Node* root, const char* png_file_name) {
  if (root == nullptr) {
    return;
  }

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

void treap::subtree2png(treap::Node* parent, treap::Node* child, const char* direction, FILE* dot_file) {
  if (child == nullptr) {
    return;
  }

  fprintf(dot_file, "node%p[label = \"{<data> data: key = %ld, priority = %ld | {<left> l: %p| <right> r: %p}}\"];\n", child,
          child->key, child->priority, child->left, child->right);

  fprintf(dot_file, "node%p:<%s>:s -> node%p:<data>:n;\n", parent, direction, child);

  subtree2png(child, child->left, "left", dot_file);
  subtree2png(child, child->right, "right", dot_file);
}

void treap::draw(treap& tree, const char* png_file_name) {
  tree2png(tree.root, png_file_name);
}
