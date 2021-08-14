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

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  int index(T x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  const T &value(int i) const { return values[i]; }
};

using P = pair<i64, i64>;

auto solve() {
  INPUT(int, n);
  vector<P> points;
  vector<array<P, 2>> edges(n);
  REP(i, n) {
    INPUT(i64, a, b, c, d);
    i64 nume1 = c * b, deno1 = d * (a + b);
    i64 g1 = gcd(nume1, deno1);
    i64 nume2 = (c + d) * b, deno2 = d * a;
    i64 g2 = gcd(nume2, deno2);
    P p1 = {nume1 / g1, deno1 / g1}, p2 = {nume2 / g2, deno2 / g2};
    edges[i] = {p1, p2};
    points.push_back(p1);
    points.push_back(p2);
  }
  Compressed<P> cp(points);
  const int node_count = cp.size();

  vector<vector<pair<int, int>>> g(node_count);
  REP(i, n) {
    auto[p1, p2] = edges[i];
    int u = cp.index(p1);
    int v = cp.index(p2);
    g[u].push_back({v, i});
    g[v].push_back({u, i});
  }

  vector<int> doneE(n, 0), depth(node_count, -1);
  vector<pair<int, int>> edge_pairs;

  auto dfs = [&](auto &dfs, int v, int peid) -> bool {
    int ec = -1;
    for (const auto &[to, eid] : g[v]) {
      if (depth[to] != -1 and depth[to] > depth[v]) continue;
      if (depth[to] == -1) {
        depth[to] = depth[v] + 1;
      }
      if (doneE[eid]) continue;
      doneE[eid] = true;
      if (depth[to] > depth[v]) {
        if (not dfs(dfs, to, eid)) continue;
      }
      if (ec == -1) {
        ec = eid;
      } else {
        edge_pairs.push_back({ec + 1, eid + 1});
        ec = -1;
      }
    }
    bool has_edge = (peid != -1);
    if (peid != -1 and ec != -1) {
      edge_pairs.push_back({peid + 1, ec + 1});
      has_edge = false;
      doneE[peid] = true;
    }
    return has_edge;
  };

  REP(v, node_count) {
    if (depth[v] != -1) continue;
    depth[v] = 0;
    dfs(dfs, v, -1);
  }
  print(ssize(edge_pairs));
  for (const auto &[u, v] : edge_pairs) {
    print(u, v);
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
