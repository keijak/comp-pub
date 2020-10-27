#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
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
void pdebug(const T &value, const Ts &... args) {
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

const i64 INF = 1e18;

// Find the shortest path from the start node and detects negative cycle.
// The first return value indicates the distance from the start node.
// The second return value indicates which nodes have a negative cycle in a
// path from the start node.
std::pair<std::vector<i64>, bool> bellman_ford(
    const std::vector<std::vector<std::pair<int, i64>>> &adj, const int start,
    std::function<bool(int)> is_active) {
  const int n = int(adj.size());
  std::vector<i64> min_dist(n, INF);  // distance from the start node.
  min_dist[start] = 0;

  for (int k = 0;; ++k) {
    bool updated = false;
    for (int i = 0; i < n; ++i) {
      if (not is_active(i)) continue;
      const auto di = min_dist[i];
      if (di == INF) continue;  // We haven't reached i yet.
      for (auto [j, dist] : adj[i]) {
        if (not is_active(j)) continue;
        if (min_dist[j] > di + dist) {
          min_dist[j] = di + dist;
          updated = true;
        }
      }
    }

    if (not updated) {
      return {std::move(min_dist), false};
    }

    // Negative cycle detection
    if (k >= n) {
      // When there's no negative cycle, at least one node gets the shortest
      // distance determined in each iteration. If we have gone through N+1
      // iterations and still have an update, there must be a negative cycle.
      return {std::move(min_dist), true};
    }
  }
  assert(false);  // Never reach here.
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  V<V<pair<int, i64>>> g(n), gr(n);
  for (int i = 0; i < m; ++i) {
    int u, v;  // read as 1-indexed.
    i64 c;
    cin >> u >> v >> c;
    u--, v--;  // to 0-indexed.
    g[u].emplace_back(v, -c);
    gr[v].emplace_back(u, -c);
  }
  V<bool> reach1(n), reach2(n);
  auto dfs1 = [&](auto self, int v) -> void {
    for (auto [u, _] : g[v]) {
      if (reach1[u]) continue;
      reach1[u] = true;
      self(self, u);
    }
  };
  reach1[0] = true;
  dfs1(dfs1, 0);
  auto dfs2 = [&](auto self, int v) -> void {
    for (auto [u, _] : gr[v]) {
      if (reach2[u]) continue;
      reach2[u] = true;
      self(self, u);
    }
  };
  reach2[n - 1] = true;
  dfs2(dfs2, n - 1);

  auto [dist, neg_inf] =
      bellman_ford(g, 0, [&](int v) { return reach1[v] and reach2[v]; });
  if (neg_inf) {
    cout << "inf" << endl;
  } else {
    assert(dist[n - 1] != INF);  // reachable.
    cout << -dist[n - 1] << endl;
  }
}
