#include<unordered_map>

template<typename T>
class set_find {
private:
  std::unordered_map<T, T> p;
  std::unordered_map<T, T> rank;
public:
  void MakeSet(T x) {
    p[x] = x;
    rank[x] = 0;
  }

  T Find(T x) {
    if (p[x] == x) {return x;}
    p[x] = Find(p[x]);
    return p[x];
  }

  void Unite(T x, T y) {
    x = Find(x);
    y = Find(y);
    if (rank[x] < rank[y])
      p[x] = y;
    else
    {
      p[y] = x;
      if (rank[x] == rank[y])
        ++rank[x];
    }
  }
};
