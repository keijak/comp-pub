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

void print() { std::cout << "\n"; }
template <class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template <typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template <typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define VAR(type, ...) \
  type __VA_ARGS__;    \
  read_from_cin(__VA_ARGS__);

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template <class T>
T floor_mod(T x, T y) {
  assert(y > 0);
  auto r = x % y;
  if (r < 0) r += y;
  return r;
}

auto solve() -> bool {
  VAR(int, n, m);
  vector<i64> vals(n), targets(n);
  cin >> vals >> targets;
  vector<vector<int>> g(n);
  REP(i, m) {
    VAR(int, u, v);
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  i64 vals_sum = accumulate(ALL(vals), 0LL);
  i64 targets_sum = accumulate(ALL(targets), 0LL);
  if ((vals_sum & 1) != (targets_sum & 1)) {
    return false;
  }

  vector<char> visited(n, 0);
  vector<int> colors(n, 0);
  auto dfs = [&](auto &rec, int v, int color) -> bool {
    int next_color = 3 - color;
    for (auto u : g[v]) {
      if (visited[u]) {
        if (colors[u] != next_color) return false;
        continue;
      }
      visited[u] = true;
      colors[u] = next_color;
      bool sub = rec(rec, u, colors[u]);
      if (not sub) return false;
    }
    return true;
  };
  visited[0] = true;
  colors[0] = 1;
  bool bipartite = dfs(dfs, 0, 1);
  if (not bipartite) return true;

  REP(i, n) if (not visited[i]) return false;

  DUMP(colors);
  i64 s1 = 0, s2 = 0;
  REP(i, n) {
    if (colors[i] == 1) {
      s1 += targets[i] - vals[i];
    } else if (colors[i] == 2) {
      s2 += targets[i] - vals[i];
      DUMP(i, s2);
    } else {
      assert(false);
    }
  }
  return s1 == s2;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  VAR(int, t);
  REP(test_case, t) {
    auto ans = solve();
    print(ans ? "YES" : "NO");
  }
}
