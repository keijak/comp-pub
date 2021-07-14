#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

bool solve(vector<int> v) {
  map<int, int, greater<int>> freq;
  for (auto x : v) {
    freq[x]++;
  }
  while (freq.size()) {
    auto it = freq.begin();  // max
    int m = it->first;
    // DUMP(m);
    if (it->second == 1) {
      it = freq.erase(it);
    } else {
      it->second--;
    }
    vector<pair<int, int>> updates;
    while (it != freq.end() and m > 0) {
      int r = min(it->second, m);
      updates.emplace_back(it->first, r);
      m -= r;
      ++it;
    }
    if (m > 0) return false;
    reverse(ALL(updates));
    for (const auto &[k, cnt] : updates) {
      if (freq[k] > cnt) {
        freq[k] -= cnt;
      } else {
        freq.erase(k);
      }
      if (k - 1 > 0) {
        freq[k - 1] += cnt;
      }
    }
    // DUMP(freq);
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  string line;
  while (getline(cin, line)) {
    stringstream ss(line);
    vector<int> v;
    int x;
    while (ss >> x) v.push_back(x);
    cout << (solve(move(v)) ? "ok" : "fail") << "\n\n";
  }
}
