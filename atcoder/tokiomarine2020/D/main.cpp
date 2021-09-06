#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
  for (auto &x: a) is >> x;
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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

auto main() -> int {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n = in;
  vector<i64> V(n + 1), W(n + 1);
  REP(i, n) {
    int v = in, w = in;
    V[i + 1] = v;
    W[i + 1] = w;
  }
  const int Q = in;
  vector<pair<int, int>> queries(Q);
  int lmax = 0;
  REP(i, Q) {
    int v = in, L = in;
    queries[i] = {v, L};
    chmax(lmax, L);
  }
  const int X = 9;
  const int PX = 1 << X;
  vector<vector<i64>> dp(min(PX, n + 1), vector<i64>(lmax + 1, 0));

  auto dfs = [&](auto &f, int v, int d) -> void {
    if (v > n or d > X) return;
    int par = v / 2;
    auto &mp = dp[v] = dp[par];
    for (int w = 0; w + W[v] <= lmax; ++w) {
      int w2 = w + W[v];
      chmax(mp[w2], dp[par][w] + V[v]);
    }
    for (int w = 1; w <= lmax; ++w) {
      chmax(mp[w], mp[w - 1]);
    }
    for (int u: {2 * v, 2 * v + 1}) {
      f(f, u, d + 1);
    }
  };
  dfs(dfs, 1, 1);

  auto query = [&](auto &f, int v, i64 limit) -> i64 {
    if (v < PX) {
      return dp[v][limit];
    }
    int par = v / 2;
    i64 v1 = f(f, par, limit);
    if (limit - W[v] >= 0) {
      i64 v2 = f(f, par, limit - W[v]) + V[v];
      chmax(v1, v2);
    }
    return v1;
  };

  for (const auto&[v, L]: queries) {
    print(query(query, v, L));
  }
}
