#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using Float = long double;

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

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int K;
  cin >> K;
  const i64 n = 1LL << K;
  vector<int> R(n);
  REP(i, n) cin >> R[i];
  auto dp = vector(K + 1, vector(n, Float(0)));
  REP(i, n) { dp[0][i] = 1.0; }

  REP(k, 1, K + 1) {
    int mask = 0;
    REP(i, k - 1, K + 1) { mask |= (1 << i); }
    REP(i, n) {
      dp[k][i] = dp[k - 1][i];
      Float win = 0;
      REP(j, n) if (((j ^ i) & mask) == (1 << (k - 1))) {
        Float pr =
            1.0 / (1.0 + pow(Float(10.0), Float(R[j] - R[i]) / Float(400)));
        win += dp[k - 1][j] * pr;
      }
      dp[k][i] *= win;
    }
  }
  vector<Float> ans(n);
  REP(i, n) { ans[i] = dp[K][i]; }
  REP(i, n) { cout << ans[i] << "\n"; }
}
