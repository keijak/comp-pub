#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

struct DoublingAncestor {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G adj;
  vector<vector<int>> upper;  // 2^k upper node
  vector<int> depth;

  explicit DoublingAncestor(G g, int root = 0)
      : n(g.size()), adj(move(g)), upper(K, vector<int>(n, -1)), depth(n) {
    depth[root] = 0;
    dfs(0, -1);
    for (int k = 0; k + 1 < K; k++) {
      for (int v = 0; v < n; ++v) {
        if (upper[k][v] < 0) {
          upper[k + 1][v] = -1;
        } else {
          upper[k + 1][v] = upper[k][upper[k][v]];
        }
      }
    }
  }

  // Returns the node ID of the lowest common ancestor.
  int lca(int u, int v) const {
    if (depth[u] > depth[v]) swap(u, v);
    for (int k = 0; k < K; k++) {
      if ((depth[v] - depth[u]) & (1 << k)) {
        v = upper[k][v];
      }
    }
    if (u == v) return u;
    for (int k = K - 1; k >= 0; --k) {
      if (upper[k][u] != upper[k][v]) {
        u = upper[k][u];
        v = upper[k][v];
      }
    }
    return upper[0][u];
  }

  int parent(int v) const { return upper[0][v]; }

  // k-th ancestor of v.
  int ancestor(int v, int k) const {
    for (int i = K - 1; i >= 0; --i) {
      if (k & (1 << i)) {
        v = upper[i][v];
      }
    }
    return v;
  }

  // Returns the distance (number of edges) between two nodes.
  int distance(int u, int v) const {
    int p = lca(u, v);
    return (depth[u] - depth[p]) + (depth[v] - depth[p]);
  }

 private:
  void dfs(int v, int p) {
    upper[0][v] = p;  // parent
    for (auto u : adj[v]) {
      if (u == p) continue;
      depth[u] = depth[v] + 1;
      dfs(u, v);
    }
  }

  static const int K = 30;  // max upper lookup (2^K)
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n;
  cin >> n;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  DoublingAncestor da(g);

  vector<int> subsize(n, 0);
  auto dfs_sz = [&](auto &rec, int v, int p) -> int {
    int res = 1;
    for (int u : g[v]) {
      if (u == p) continue;
      res += rec(rec, u, v);
    }
    return subsize[v] = res;
  };
  dfs_sz(dfs_sz, 0, -1);

  auto count_rooms = [&](int x, int y) -> int {
    if (x == y) return n;

    int d = da.distance(x, y);
    if (d & 1) return 0;

    int dx = da.depth[x];
    int dy = da.depth[y];
    if (dx == dy) {
      int lca = da.lca(x, y);
      int dl = da.depth[lca];
      int ans = n;
      {
        int u = da.ancestor(x, dx - dl - 1);
        ans -= subsize[u];
      }
      {
        int u = da.ancestor(y, dy - dl - 1);
        ans -= subsize[u];
      }
      return ans;
    }

    if (dx > dy) {
      swap(x, y);
      swap(dx, dy);
    }

    int mid = da.ancestor(y, d / 2);
    DEBUG(mid);
    int u = da.ancestor(y, d / 2 - 1);
    int ans = subsize[mid] - subsize[u];
    return ans;
  };

  int m;
  cin >> m;
  REP(i, m) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    cout << count_rooms(x, y) << "\n";
  }
}
