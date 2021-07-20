#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
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
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

auto solve() -> Mint {
  INPUT(int, n);
  if (n == 1) return 1;
  const int L = n + 10;
  auto dp = vector(n, vector(L, vector(2, vector(2, Mint(0)))));
  dp[1][0][0][0] = 2;
  REP(i, 2, n) {
    int slots = i + 1; // number of insertion points
    REP(j, L) {  // due
      if (j > slots) break;
      REP(p1, 2) {
        REP(p2, 2) {
          auto f = dp[i - 1][j][p1][p2];
          if (f.val() == 0) continue;
          if (p1 == 0 and p2 == 0) {
            if (j + 1 < L) dp[i][j + 1][1][0] += f * 2;
            if (j - 1 >= 0) dp[i][j - 1][0][0] += f * j;
            if (slots - 2 - j > 0) dp[i][j][0][0] += f * (slots - 2 - j);
          } else if (p1 == 0 and p2 == 1) {
            if (j + 1 <= n) dp[i][j + 1][1][0] += f;
            dp[i][j][1][0] += f;
            if (j - 1 >= 0) dp[i][j - 1][0][0] += f * (j - 1);
            if (slots - 1 - j > 0) dp[i][j][0][0] += f * (slots - 1 - j);
          } else if (p1 == 1 and p2 == 0) {
            if (j + 1 < L) dp[i][j + 1][1][1] += f * 2;
            if (j - 1 >= 0) dp[i][j - 1][0][0] += f;
            if (j - 1 >= 0) dp[i][j - 1][0][1] += f * (j - 1);
            if (slots - 2 - j > 0) dp[i][j][0][1] += f * (slots - 2 - j);
          } else {
            if (j + 1 < L) dp[i][j + 1][1][1] += f;
            dp[i][j][1][1] += f;
            if (j - 1 >= 0) dp[i][j - 1][0][0] += f;
            if (j - 2 >= 0) dp[i][j - 1][0][1] += f * (j - 2);
            if (slots - 1 - j > 0) dp[i][j][0][1] += f * (slots - 1 - j);
          }
        }
      }
    }
  }
  Mint ans = dp[n - 1][0][0][0];
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
