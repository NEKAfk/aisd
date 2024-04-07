#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

static const uint64_t base = 10;

static uint64_t bin_pow(uint64_t a, std::size_t p) {
  uint64_t res = 1;
  while (p > 0) {
    if (p & 1UL) {
      res *= a;
      p--;
    }
    a *= a;
    p /= 2;
  }
  return res;
}

static std::size_t get_len(uint64_t n) {
  if (n == 0) {
    return 1;
  }
  std::size_t res = 0;
  while (n) {
    res++;
    n/=base;
  }
  return res;
}

static uint64_t get_digit(uint64_t number, std::size_t pos) {
  return (number / bin_pow(base, pos)) % base;
}

void lsd(std::vector<uint64_t>& nums) {
  std::size_t m_len = get_len(*std::max_element(nums.begin(), nums.end()));
  std::vector<uint64_t> copy(nums.size());
  for (std::size_t i = 0; i < m_len; i++) {
    std::vector<std::size_t> cnt(base + 1, 0);
    for (auto n : nums) {
      cnt[get_digit(n, i) + 1]++;
    }
    for (std::size_t k = 1; k <= base; k++) {
      cnt[k] += cnt[k - 1];
    }
    for (auto n : nums) {
      copy[cnt[get_digit(n, i)]++] = n;
    }
    std::swap(nums, copy);
  }
}