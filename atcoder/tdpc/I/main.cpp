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

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

auto solve() {
  string s;
  cin >> s;
  const int n = s.size();
  vector<int> wcount(n + 1, 0);
  REP(i, n) { wcount[i + 1] = wcount[i] + int(s[i] == 'w'); }
  // dp[l][r] := max iwis in [l,r)
  auto dp = vector(n + 1, vector(n + 1, 0LL));
  for (int width = 3; width <= n; ++width) {
    for (int l = 0; l + width <= n; ++l) {
      const int r = l + width;
      chmax(dp[l][r], dp[l + 1][r]);
      if (s[l] == 'i') {
        for (int m = l + 2; m < r; ++m) {
          if (s[m] != 'i') continue;
          int w = m - l + 1;
          if (w % 3 != 0) continue;
          if (w / 3 != wcount[m] - wcount[l]) continue;
          if (w != dp[l + 1][m] * 3 + 3) continue;
          chmax(dp[l][r], (dp[l + 1][m] + 1) + dp[m + 1][r]);
        }
      }
    }
  }
  return dp[0][n];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
