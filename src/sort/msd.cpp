#include <iostream>
#include <vector>

static const size_t n = 3, k = 10;

static int32_t getDigit(std::vector<int32_t>& item, std::size_t d) {
  return item[d];
}

void msd(std::vector<std::vector<int32_t>*>& vec, std::size_t l, std::size_t r, std::size_t d) {
  if (d == n || r <= 1 + l) {
    return;
  }
  std::vector<int32_t> count(k + 1, 0);
  std::vector<std::vector<int32_t>*> cop(r - l);
  std::cout << d << ' ' << l << ' ' << r << " OK\n";
  for (std::size_t i = l; i < r; i++) {
    count[getDigit(*vec[i], d) + 1]++;
  }
  for (std::size_t i = 2; i < k + 1; i++) {
    count[i] += count[i - 1];
  }
  for (std::size_t i = l; i < r; i++) {
    std::size_t j = getDigit(*vec[i], d);
    cop[count[j]] = vec[i];
    count[j]++;
  }
  for (std::size_t i = l; i < r; i++) {
    vec[i] = cop[i - l];
  }
  msd(vec, l, l + count[0], d + 1);
  for (std::size_t i = 1; i < k + 1; i++) {
    msd(vec, l + count[i - 1], l + count[i], d + 1);
  }
}