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

vector<int> solve() {
  int n;
  cin >> n;
  vector<int> p(n + 1);
  REP(i, 1, n + 1) { cin >> p[i]; }
  DUMP(p);
  vector<int> ans;
  for (int i = 1;; ++i) {
    DUMP(i, p);
    if (std::is_sorted(p.begin(), p.end())) {
      return ans;
    }
    int q = -1, j0 = n - 1, rbig = -1;
    if ((i % 2) != (j0 % 2)) --j0;
    for (int j = j0; j >= 1; j -= 2) {
      if (p[j] > j and p[j] > rbig) {
        q = j;
        rbig = p[j];
        break;
      }
    }
    if (q != -1) {
      ans.push_back(q);
      swap(p[q], p[q + 1]);
    } else {
      int q0 = 1;
      if ((q0 % 2) != (i % 2)) ++q0;
      ans.push_back(q0);
      swap(p[q0], p[q0 + 1]);
    }
  }
  assert(false);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int T;
  cin >> T;
  REP(i, T) {
    auto res = solve();
    cout << res.size() << "\n";
    print_seq(res);
  }
}
