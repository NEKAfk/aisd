#include <iostream>
#include <vector>

// снизу вверх
void merge_sort(std::vector<int32_t>& vec) {
  std::vector<int32_t> cop(vec.size());
  for (size_t i = 1; i < (vec.size() + 1) / 2; i<<=1) {
    size_t lf = 0, rf = i, ls = i, rs = std::min(2 * i, vec.size());
    size_t insert_point = 0;
    while (rf < vec.size()) {
      for (size_t j = 0; j < rs - rf + i; j++, insert_point++) {
        if (lf < rf && ls < rs) {
          if (vec[lf] < vec[ls]) {
            cop[insert_point] = vec[lf];
            lf++;
          } else {
            cop[insert_point] = vec[ls];
            ls++;
          }
        } else if (lf < rf) {
          cop[insert_point] = vec[lf];
          lf++;
        } else {
          cop[insert_point] = vec[ls];
          ls++;
        }
      }
      lf = rs;
      ls += i;
      rf = ls;
      rs = std::min(ls + i, vec.size());
    }
    std::copy(cop.begin(),
              cop.begin() + static_cast<std::ptrdiff_t>(insert_point),
              vec.begin());
  }
}