#include "../../include/dst.h"

dst::precalc::precalc(size_t n) : pref(n, 0), suf(n, 0) {}

dst::precalc::precalc() : pref(0), suf(0) {}

int64_t dst::precalc::sum(size_t i, size_t j) {
  return suf[i] + pref[j];
}

void dst::build(std::vector<int64_t>& vec) {
  for (size_t i = 0; i < std::__lg(vec.size()); i++) {
    size_t p = 1LL << i;
    for (size_t k = 0; k < (vec.size() >> (i + 1)); k++) {
      precalc tmp((1LL << i) + 1);
      for (size_t j = 1; j < (1LL << i) + 1; j++) {
        tmp.pref[j] = tmp.pref[j - 1] + vec[p + j - 1];
        tmp.suf[j] = tmp.suf[j - 1] + vec[p - j];
      }
      table[p] = tmp;
      p += 1LL << (i + 1);
    }
  }
}

dst::dst(std::vector<int64_t>& vec) : table(vec.size()) {
  build(vec);
}

int64_t dst::sum(size_t l, size_t r) {
  std::size_t m = (r >> std::__lg(l ^ r)) << std::__lg(l ^ r);
  return table[m].sum(m - l, r - m);
}