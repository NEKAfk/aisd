#include<vector>
#include<algorithm>

std::pair<std::size_t, std::size_t> part(std::vector<int32_t>& nums,
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

int32_t bfprt(std::vector<int32_t>& vec, std::size_t l, std::size_t r, std::size_t k) {
  if (r-l <= 5) {
    sort(vec.begin() + static_cast<std::ptrdiff_t>(l),
         vec.begin() + static_cast<std::ptrdiff_t>(r));
    return vec[l+k];
  }
  std::size_t cnt = 0;
  for (size_t i = l; i < r; i+=5) {
    sort(vec.begin() + static_cast<std::ptrdiff_t>(i),
         vec.begin() + static_cast<std::ptrdiff_t>(std::min(i + 5, r)));
    std::swap(vec[cnt], vec[i + 2 < r ? i + 2 : i]);
    cnt++;
  }
  int32_t pivot = bfprt(vec, 0, cnt, cnt/2);
  auto [i, j] = part(vec, l, r, pivot);
  if (l+k >= i) {
    if (l+k < j) {
      return vec[l + k];
    }
    return bfprt(vec, j, r, l - j + k);
  }
  return bfprt(vec, l, i, k);
}