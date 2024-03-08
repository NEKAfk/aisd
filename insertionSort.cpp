#include <cstddef>
#include <cstdint>
#include <vector>

void insertSort(std::vector<int32_t>& vec) {
  for (size_t i = 1; i < vec.size(); i++) {
    size_t cur = i;
    while (cur > 0 && vec[cur] < vec[cur - 1]) {
      std::swap(vec[cur], vec[cur-1]);
      cur--;
    }
  }
}
