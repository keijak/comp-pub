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

#ifdef MY_DEBUG
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

// Finds the shortest path from the start node and detects negative cycle.
// Returns the distance from the start node.
// INF indicates unreachable. -INF indicates having a negative cycle in a
// path from the start node.
vector<i64> bellman_ford(const vector<vector<pair<int, i64>>> &adj,
                         const int start) {
  const int n = int(adj.size());
  const i64 INF = 1e18;
  vector<i64> min_dist(n, INF);  // distance from the start node.
  min_dist[start] = 0;

  for (int k = 0; k < n - 1; ++k) {
    for (int i = 0; i < n; ++i) {
      const auto di = min_dist[i];
      if (di == INF) continue;  // We haven't reached i yet.
      for (auto [j, dist] : adj[i]) {
        if (min_dist[j] > di + dist) {
          min_dist[j] = di + dist;
        }
      }
    }
  }

  // Negative cycle detection.
  // When there's no negative cycle, at least one node gets the shortest
  // distance determined in each iteration. If we have gone through N+1
  // iterations and still have an update, there must be a negative cycle.
  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      if (min_dist[i] == INF) continue;
      bool negative_cycle = (min_dist[i] == -INF);
      for (auto [j, dist] : adj[i]) {
        if (negative_cycle or min_dist[j] > min_dist[i] + dist) {
          min_dist[j] = -INF;
        }
      }
    }
  }

  return min_dist;
}

struct Graph {
  int n;
  V<V<pair<int, i64>>> adj;
  explicit Graph(int n) : n(n), adj(n) {}

  void input(int m) {
    for (int i = 0; i < m; ++i) {
      int u, v;  // read as 1-indexed.
      i64 c;
      cin >> u >> v >> c;
      u--, v--;  // to 0-indexed.
      adj[u].emplace_back(v, -c);
    }
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  Graph g(n);
  g.input(m);

  auto min_dist = bellman_ford(g.adj, 0);
  assert(min_dist[n - 1] != INF);  // reachable
  if (min_dist[n - 1] == -INF) {
    cout << "inf" << endl;
  } else {
    cout << -min_dist[n - 1] << endl;
  }
}
