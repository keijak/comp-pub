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
using P = pair<i64, i64>;

vector<i64> solve() {
  i64 n, m, i0, j0;
  cin >> n >> m >> i0 >> j0;

  auto pick = [&](const P &x, const P &y) -> i64 {
    i64 s = abs(x.first - i0) + abs(x.second - j0);
    s += abs(x.first - y.first) + abs(x.second - y.second);
    s += abs(y.first - i0) + abs(y.second - j0);
    return s;
  };

  vector<pair<i64, i64>> c = {{1, 1}, {n, 1}, {1, m}, {n, m}};
  i64 maxsteps = -1;
  vector<i64> ans(4, 0);
  REP(i, 4) REP(j, 4) {
    if (i == j) continue;
    i64 steps = min(pick(c[i], c[j]), pick(c[j], c[i]));
    if (chmax(maxsteps, steps)) {
      ans[0] = c[i].first;
      ans[1] = c[i].second;
      ans[2] = c[j].first;
      ans[3] = c[j].second;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    auto res = solve();
    print_seq(res);
  }
}
