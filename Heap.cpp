#include<vector>

template<typename T, typename P>
class heap {
private:
  std::vector<T> array;
  P predicate;

  void sift_down(std::size_t idx) {
    while (true) {
      std::size_t l = 2 * idx + 1;
      std::size_t r = 2 * idx + 2;
      std::size_t s = idx;
      if (l < array.size() && predicate(array[l], array[s])) {
        s = l;
      }
      if (r < array.size() && predicate(array[r], array[s])) {
        s = r;
      }
      if (s == idx) {
        break;
      }
      std::swap(array[idx], array[s]);
      idx = s;
    }
  }

  void sift_up(std::size_t idx) {
    while (idx != 0) {
      if (predicate(array[idx], array[(idx - 1) / 2])) {
        std::swap(array[(idx - 1) / 2], array[idx]);
        idx = (idx - 1) / 2;
      } else {
        break;
      }
    }
  }

  void build() {
    for (std::ptrdiff_t i = array.size() - 1; i >= 0; i--) {
      sift_down(i);
    }
  }

public:
  explicit heap(P pred) : predicate(pred) {
    array = new std::vector<T>();
  }

  explicit heap(std::vector<T> vec, P pred) : array(std::move(vec)), predicate(pred) {
    build();
  }

  void push(T elem) {
    array.push_back(elem);
    sift_up(array.size() - 1);
  }

  T top() {
    return array[0];
  }

  T pop() {
    T res = array[0];
    std::swap(array[0], array[array.size() - 1]);
    array.pop_back();
    sift_down(0);
    return res;
  }

  bool isEmpty() {
    return array.empty();
  }
};