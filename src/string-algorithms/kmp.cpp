#include <string>
#include <vector>

inline std::size_t pref_count(std::size_t init, std::string& temp, char c, std::vector<size_t>& pref) {
  std::size_t k = init;
  while (k > 0 && c != temp[k]) {
    k = pref[k - 1];
  }
  if (c == temp[k]) {
    k++;
  }
  return k;
}

std::vector<std::size_t> kmp(std::string& P, std::string& Q) {
  std::vector<std::size_t> pref(P.size(), 0);
  for (size_t i = 1; i < P.size(); i++) {
    pref[i] = pref_count(pref[i - 1], P, P[i], pref);
  }

  std::vector<std::size_t> result;
  std::size_t last = 0;
  for (std::size_t i = 0; i < Q.size(); i++) {
    last = pref_count(last, P, Q[i], pref);
    if (last == P.size()) {
      result.push_back(i - P.size() + 1);
    }
  }
  return result;
}