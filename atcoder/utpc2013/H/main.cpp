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
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__);

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

const i64 INF = std::numeric_limits<i64>::max() / 2;

struct Edge {
  i64 cost;
  int to;
};

// Finds the shortest path from the start node and detects negative cycle if
// exists. Returns the minimum cost from the start node to each node. INF
// indicates unreachable. -INF indicates having a negative cycle in a path from
// the start node.
auto bellman_ford(const std::vector<std::vector<Edge>> &adj, const int start) {
  const int n = int(adj.size());
  std::vector<i64> mincost(n, INF);
  mincost[start] = 0;

  for (int k = 0; k < n - 1; ++k) {
    for (int i = 0; i < n; ++i) {
      const i64 di = mincost[i];
      if (di == INF) continue;  // Haven't reached i yet.
      for (const Edge &e : adj[i]) {
        if (mincost[e.to] > di + e.cost) {
          mincost[e.to] = di + e.cost;
        }
      }
    }
  }

  // Negative cycle detection.
  // If there's no negative cycle, at least one node gets the shortest
  // distance determined in each iteration above. If we have gone through N-1
  // iterations and still have an update, there must be a negative cycle.
  for (int k = 0; k < n; ++k) {
    bool updated = false;
    for (int i = 0; i < n; ++i) {
      const i64 di = mincost[i];
      if (di == INF) continue;
      const bool in_negative_cycle = (di == -INF);
      for (const Edge &e : adj[i]) {
        if (mincost[e.to] == -INF) continue;
        if (in_negative_cycle or (mincost[e.to] > di + e.cost)) {
          mincost[e.to] = -INF;
          updated = true;
        }
      }
    }
    if (not updated) break;
  }

  return mincost;
}

auto solve() {
  INPUT(int, n, m);
  vector<int> p(n + 1), q(n + 1);
  REP(i, n) cin >> p[i + 1];
  REP(i, n) cin >> q[i + 1];
  vector<vector<Edge>> g(2 * n + 1);
  const int Z = 0;
  REP(i, 1, n + 1) {
    g[i].push_back({0, Z});
    g[Z].push_back({p[i], i});
    g[n + i].push_back({q[i], Z});
    g[Z].push_back({0, n + i});
  }
  REP(i, m) {
    INPUT(int, x, y, a, b);
    g[n + y].push_back({b, x});
    g[x].push_back({-a, n + y});
  }
  auto res = bellman_ford(g, Z);
  DUMP(res);
  REP(i, 1, n + 1) {
    if (res[i] == -INF) return false;
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans ? "yes" : "no");
  }
}
