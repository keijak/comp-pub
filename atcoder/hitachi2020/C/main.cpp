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

auto solve() -> optional<vector<int>> {
  int n = in;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int a = in, b = in;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  int k = n / 3;
  int n1 = k, n2 = k, n0 = k;
  if (n % 3 == 2) n2++;
  if (n % 3 == 1 or n % 3 == 2) n1++;
  DUMP(n1, n2, n0);

  vector<int> depth(n, -1);
  vector<vector<int>> id(2);

  auto dfs = [&](auto &f, int v, int p, int d) -> void {
    depth[v] = d;
    id[d % 2].push_back(v);
    for (auto u: g[v]) {
      if (u == p) continue;
      f(f, u, v, d + 1);
    }
  };
  dfs(dfs, 0, -1, 0);
  DUMP(depth);
  DUMP(id[0]);
  DUMP(id[1]);
  int ge = 1;
  if (id[0].size() > id[1].size()) ge = 0;
  DUMP(ge);
  int le = (ge + 1) % 2;

  vector<int> p(n, -1);
  int c0 = 0;

  if (ssize(id[le]) <= n0) {
    REP(i, ssize(id[le])) {
      int v = id[le][i];
      p[v] = 3 * ++c0;
    }
    REP(i, ssize(id[ge])) {
      int v = id[ge][i];
      if (i < n1) {
        p[v] = 3 * i + 1;
      } else if (i - n1 < n2) {
        p[v] = 3 * (i - n1) + 2;
      } else {
        p[v] = 3 * ++c0;
      }
    }
    return p;
  }

  if (ssize(id[ge]) > n1 + n0 or ssize(id[le]) > n2 + n0) {
    return nullopt;
  }

  REP(i, ssize(id[ge])) {
    int v = id[ge][i];
    if (i < n1) {
      p[v] = 3 * i + 1;
    } else {
      p[v] = 3 * (++c0);
    }
  }
  REP(i, ssize(id[le])) {
    int v = id[le][i];
    if (i < n2) {
      p[v] = 3 * i + 2;
    } else {
      p[v] = 3 * (++c0);
    }
  }
  assert(c0 == n0);
  return p;
}

auto main() -> int {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    if (not ans) {
      print(-1);
    } else {
      print_seq(ans.value());
    }
  }
}
