#include<vector>

template<typename T, typename P>
int bin_search(std::vector<T>& vec, int l, int r, P pred) {
  while (r - l > 1) {
    int m = l + (r - l) / 2;
    if (pred(vec[m])) {
      l = m;
    } else {
      r = m;
    }
  }
  return r;
}