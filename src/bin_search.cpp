#include<vector>

template<typename T, typename P>
std::ptrdiff_t bin_search(std::vector<T>& vec, std::ptrdiff_t l, std::ptrdiff_t r, P pred) {
  while (r - l > 1) {
    std::ptrdiff_t m = l + (r - l) / 2;
    if (pred(vec[m])) {
      l = m;
    } else {
      r = m;
    }
  }
  return r;
}