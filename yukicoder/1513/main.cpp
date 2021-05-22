#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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

void solve() {
  int n, k;
  cin >> n >> k;

  auto dp1 = vector(n + 1, vector(k, vector(k, Mint(0))));
  auto dp2 = vector(n + 1, vector(k, vector(k, Mint(0))));
  REP(i, k) REP(j, k) {
    if (i == j) continue;
    dp1[2][i][j] = 1;
    dp2[2][i][j] = i + j;
  }
  REP(i, 2, n) {
    REP(q, k) {
      Mint acc = 0, accs = 0;
      for (int p = 0; p < q; ++p) {
        acc += dp1[i][p][q];
        accs += dp2[i][p][q];
      }
      for (int r = 0; r < q; ++r) {
        dp1[i + 1][q][r] += acc - dp1[i][r][q];
        dp2[i + 1][q][r] += accs - dp2[i][r][q];
        dp2[i + 1][q][r] += r * (acc - dp1[i][r][q]);
      }
      acc = accs = 0;
      for (int p = q + 1; p < k; ++p) {
        acc += dp1[i][p][q];
        accs += dp2[i][p][q];
      }
      for (int r = q + 1; r < k; ++r) {
        dp1[i + 1][q][r] += acc - dp1[i][r][q];
        dp2[i + 1][q][r] += accs - dp2[i][r][q];
        dp2[i + 1][q][r] += r * (acc - dp1[i][r][q]);
      }
    }
  }
  Mint ans1 = 0, ans2 = 0;
  REP(p, k) REP(q, k) {
    ans1 += dp1[n][p][q];
    ans2 += dp2[n][p][q];
  }
  cout << ans1 << " " << ans2 << endl;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
