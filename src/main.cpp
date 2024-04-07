#include "../include/treap_key.h"

#include <iostream>
#include <vector>

void lsd(std::vector<uint64_t>& nums);

void msd(std::vector<uint64_t>& nums);

int main() {
  std::vector<int64_t> init = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  treap_key Treap(init);
  treap_key l, r;
  Treap.reverse(4, 10);
  treap_key::split(Treap, 4, l, r);
  treap_key newTreap;
  treap_key::merge(r, l, newTreap);
  newTreap.add(10, 12, -100);
  newTreap.set(0, 2, 0);
  treap_key::draw(newTreap, "0.png");

  std::vector<uint64_t> vec = {68,489,4371,2,8,58,457,6,5,17,895,87148,58,785,64,8,3,75,6,813,4,54};
  msd(vec);
  for (auto n : vec) {
    std::cout << n << ' ';
  }
  std::cout << '\n';
  return 0;
}