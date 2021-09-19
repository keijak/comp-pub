#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
#include <atcoder/modint>
using Mint = atcoder::modint;
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
  return (int) a.size();
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
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
    is_iterable<T>::value && !std::is_same_v<T, std::string>>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

auto solve() {
  const int n = in, K = in, M = in;
  Mint::set_mod(M);
  const i64 S = i64(K) * n * (1 + n) / 2;
  auto dp = vector(2, vector(K * n + 1, vector(S + 1, Mint(0))));
  dp[0][0][0] = 1;
  for (int i = 0; i < n; ++i) {
    REP(c, K * i + 1) {
      REP(s, K * i * (i + 1) / 2 + 1) {
        if (dp[0][c][s].val() == 0) continue;
        REP(j, K + 1) {
          i64 c2 = c + j;
          i64 s2 = s + i64(j) * (i + 1);
          if (c2 <= K * n and s2 <= S) {
            dp[1][c2][s2] += dp[0][c][s];
          }
        }
      }
    }
    swap(dp[0], dp[1]);
    REP(c, K * n + 1) fill(ALL(dp[1][c]), 0);
  }
  vector<Mint> ans(n + 1, 0);
  REP(i, 1, K * n + 1) REP(j, 1, S + 1) {
      if (dp[0][i][j].val() == 0) continue;
      if (j % i != 0) continue;
      int x = j / i;
      ans[x] += dp[0][i][j];
    }
  for (int i = 1; i <= n; ++i) {
    print(ans[i]);
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  solve();
}
