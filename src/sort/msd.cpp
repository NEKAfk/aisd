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

static void msd_impl(const std::vector<uint64_t>& nums,
                     std::vector<std::size_t>& positions, std::vector<std::size_t>& copy,
                     std::size_t left, std::size_t right, std::size_t pos) {
  if (pos == 0 || left + 1 >= right) {
    return;
  }
  std::vector<std::size_t> cnt(base + 1, 0);
  for (std::size_t i = left; i < right; i++) {
    cnt[get_digit(nums[positions[i]], pos - 1)]++;
  }
  for (std::size_t j = 1; j <= base; j++) {
    cnt[j] += cnt[j - 1];
  }
  for (std::size_t i = left; i < right; i++) {
    copy[left + --cnt[get_digit(nums[positions[i]], pos - 1)]] = positions[i];
  }
  for (std::size_t i = left; i < right; i++) {
    positions[i] = copy[i];
  }
  for (std::size_t i = 0; i < base; i++) {
    msd_impl(nums, positions, copy, left + cnt[i], left + cnt[i + 1], pos - 1);
  }
}

void msd(std::vector<uint64_t>& nums) {
  std::vector<std::size_t> positions(nums.size());
  std::vector<std::size_t> copy(nums.size(),0);
  for (std::size_t i = 0; i < nums.size(); i++) {
    positions[i] = i;
  }
  std::size_t m_len = 0;
  for (auto& elem : nums) {
    m_len = std::max(m_len, get_len(elem));
  }
  msd_impl(nums, positions, copy, 0, nums.size(), m_len);
  std::vector<uint64_t> tmp(nums.size());
  for (std::size_t i = 0; i < nums.size(); i++) {
    tmp[i] = nums[positions[i]];
  }
  nums = std::move(tmp);
}