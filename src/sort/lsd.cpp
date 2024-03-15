#include <cstddef>
#include <cstdint>
#include <vector>

static const size_t n = 10, k = 2;

static int32_t getDigit(std::vector<int32_t>& item, std::size_t d) {
  return item[d];
}

void lsd(std::vector<std::vector<int32_t>*>& vec) {
  for (int i = 0; i < n; i++) {
    std::vector<uint32_t> bucket(k, 0);
    std::vector<std::vector<int32_t>*> cop(vec.size());
    for (auto item : vec) {
      bucket[getDigit(*item, i)]++;
    }
    uint32_t count = 0;
    for (std::size_t j = 0; j < k; j++) {
      uint32_t tmp = bucket[j];
      bucket[j] = count;
      count += tmp;
    }
    for (auto item : vec) {
      std::size_t d = getDigit(*item, i);
      cop[d] = item;
      bucket[d]++;
    }
    std::copy(cop.begin(), cop.end(), vec.begin());
  }
}