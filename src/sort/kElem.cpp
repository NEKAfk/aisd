#include <cstdint>
#include <vector>

static std::pair<std::size_t, std::size_t> partition(std::vector<int32_t>& nums,
                                              std::size_t l, std::size_t r,
                                              int32_t pivot) {
  std::size_t i = l, j = l, h = r - 1;
  while (j <= h) {
    if (nums[j] < pivot) {
      std::swap(nums[i], nums[j]);
      j++;
      i++;
    } else if (nums[j] == pivot) {
      j++;
    } else {
      std::swap(nums[j], nums[h]);
      h--;
    }
  }
  return std::pair<int32_t, int32_t>{i, j};
}

int32_t kth(std::vector<int32_t>& nums, std::size_t l, std::size_t r, std::size_t k) {
  int32_t pivot = nums[l + (r - l)/2];
  auto [i, j] = partition(nums, l, r, pivot);
  if (l+k >= i) {
    if (l+k < j) {
      return nums[l+k];
    }
    return kth(nums, j, r, l - j + k);
  }
  return kth(nums, l, i, k);
}