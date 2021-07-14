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
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// Lowest Common Ancestor by binary lifting.
// - query: O(log N)
// - build: O(N log N)
// - space: O(N log N)
struct AncestorBinaryLifting {
  using G = vector<vector<int>>;

  const int n;         // number of nodes
  const int max_bits;  // max upper lookup (2^max_bits)
  const int root;      // root node
  const G &adj;
  vector<vector<int>> upper;  // 2^k upper node
  vector<int> depth;

  explicit AncestorBinaryLifting(const G &g, int root = 0)
      : n(g.size()),
        max_bits(ceil_power2(n + 1)),
        root(root),
        adj(g),
        upper(max_bits, vector<int>(n)),
        depth(n, 0) {
    // Build `depth` and `upper[0]`.
    // The root node's parent is itself.
    dfs(root, root);

    // Build `upper[k]` for k > 0.
    for (int k = 0; k + 1 < max_bits; ++k) {
      for (int v = 0; v < n; ++v) {
        upper[k + 1][v] = upper[k][upper[k][v]];
      }
    }
  }

  // Returns the parent of v.
  // The root's parent is the root itself.
  int parent(int v) const { return upper[0][v]; }

  // Returns i-th ancestor of v.
  // - 0th ancestor is v itself.
  // - 1st ancestor is the parent.
  // - (2^k)-th ancestor is `upper[k][v]`.
  int ancestor(int v, int i) const {
    int x = v;
    for (int k = max_bits - 1; k >= 0; --k) {
      if (i & (1 << k)) {
        x = upper[k][x];
      }
    }
    return x;
  }

  // Returns the node ID of the lowest common ancestor.
  int lca(int u, int v) const {
    if (depth[u] > depth[v]) swap(u, v);

    // Move up `v` so both nodes have the same depth.
    int ddiff = depth[v] - depth[u];
    v = ancestor(v, ddiff);
    if (u == v) return u;

    // Move up both nodes but still keep them below the LCA.
    for (int k = max_bits - 1; k >= 0; --k) {
      if (upper[k][u] != upper[k][v]) {
        u = upper[k][u];
        v = upper[k][v];
      }
    }
    // Now both nodes are direct children of the LCA.
    const int pu = parent(u), pv = parent(v);
    assert(pu == pv);
    return pu;
  }

  // Returns the distance (number of edges) between two nodes.
  int distance(int u, int v) const {
    int p = lca(u, v);
    return (depth[u] - depth[p]) + (depth[v] - depth[p]);
  }

  // Binary search.
  // Returns the minimum number of steps to reach an ancestor
  // that satisfies `pred(a)`.
  template <class F>
  optional<int> min_steps(int start, F pred) const {
    static_assert(std::is_invocable_r_v<bool, F, int>);
    if (pred(start)) return 0;
    if (not pred(root)) return nullopt;
    int max_false = 0;
    int v = start;
    for (int d = max_bits - 1; d >= 0; --d) {
      auto u = upper[d][v];
      if (pred(u)) continue;
      max_false += 1 << d;
      v = u;
    }
    int min_true = max_false + 1;
    return min_true;
  }

 private:
  static int ceil_power2(int n) {
    int b = 1;
    while ((1u << b) < (unsigned)n) ++b;
    return b;
  }

  void dfs(int v, int p) {
    upper[0][v] = p;  // parent
    for (auto u : adj[v]) {
      if (u == p) continue;
      depth[u] = depth[v] + 1;
      dfs(u, v);
    }
  }
};

auto solve() {
  INPUT(int, n, Q);
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    INPUT(int, a, b);
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  AncestorBinaryLifting abl(g);
  REP(i, Q) {
    INPUT(int, c, d);
    --c, --d;
    int dis = abl.distance(c, d);
    if (dis & 1) {
      print("Road");
    } else {
      print("Town");
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) { solve(); }
}
