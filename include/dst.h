#ifndef AISD_DST_H
#define AISD_DST_H

#include <iostream>
#include <vector>

class dst {
private:
  struct precalc {
    explicit precalc(size_t n);

    precalc();

    std::vector<int64_t> pref, suf;

    int64_t sum(size_t i, size_t j);
  };

  void build(std::vector<int64_t>& vec);

  std::vector<precalc> table;

public:
  explicit dst(std::vector<int64_t>& vec);

  int64_t sum(size_t l, size_t r);
};
#endif // AISD_DST_H