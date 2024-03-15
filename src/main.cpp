#include "../include/dec_tree.h"
#include "../include/pers_seg_tree.h"

#include <iostream>
#include <vector>

int main() {
  std::size_t n; std::cin >> n;
  std::vector<int64_t> vec(n);
  for (std::size_t i = 0; i < n; i++) {
    vec[i] = i + 1;
  }
  dec_tree dt(vec);

  std::size_t cnt = 0;
  while (true) {
    std::cout << "Enter: ";
    int x = 0; std::cin >> x;
    if (x == -1) {
      break;
    }
    int64_t val = 0; std::cin >> val;
    if (x == 0) {
      dt.insert(val);
    } else {
      dt.remove(val);
    }
    dt.draw((std::to_string(cnt) + ".png").c_str());
    cnt++;
  }
}