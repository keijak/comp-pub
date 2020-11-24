#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
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

const i64 INF = 1e18;

template <class T>
using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

struct Edge {
  i64 cost;
  int to;
  int from;
  bool erased;
};

struct State {
  i64 cost;
  int node;
  //...
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
pair<V<i64>, V<int>> dijkstra(const std::vector<std::vector<Edge>> &g,
                              int source, int sink, i64 erased_cost) {
  const int n = g.size();
  std::vector<i64> mincost(n, INF);
  mincost[source] = 0LL;
  V<int> parent(n, -1);
  MinHeap<State> que;
  que.push({0LL, source});
  while (not que.empty()) {
    State cur = std::move(que.top());
    que.pop();
    if (cur.cost != mincost[cur.node]) {
      continue;
    }
    if (cur.node == sink) break;
    for (const auto &e : g[cur.node]) {
      i64 new_cost = cur.cost;
      if (e.cost == 0) {
        new_cost += erased_cost;
      } else {
        new_cost += e.cost;
      }
      if (mincost[e.to] > new_cost) {
        mincost[e.to] = new_cost;
        parent[e.to] = cur.node;
        que.push({new_cost, e.to});
      }
    }
  }
  return {mincost, parent};
}

optional<V<Edge>> solve() {
  i64 n, m, L, s, t;
  cin >> n >> m >> L >> s >> t;
  V<V<Edge>> g(n);
  V<Edge> edges(m);
  map<pair<int, int>, int> edge_id;
  REP(i, m) {
    int u, v, w;
    cin >> u >> v >> w;
    Edge e = {w, v, u, w == 0};
    g[u].push_back(e);
    g[v].push_back({w, u, v, w == 0});
    edges[i] = move(e);
    edge_id[{u, v}] = edge_id[{v, u}] = i;
  }

  auto [res2, _] = dijkstra(g, s, t, INF);
  DEBUG(res2);
  if (res2[t] < L) {
    return nullopt;
  }
  if (res2[t] == L) {
    for (auto &e : edges) {
      if (e.erased) e.cost = 1e18;
    }
    return edges;
  }

  auto [res1, par] = dijkstra(g, s, t, 1);
  DEBUG(res1);
  if (res1[t] > L) {
    return nullopt;
  }
  V<i64> cap(n, 0);
  V<int> path;
  int num_erased = 0;
  i64 pcap = INF;
  for (int v = t; v >= 0; v = par[v]) {
    cap[v] = min(res2[v] - res1[v], pcap);
    pcap = cap[v];
    path.push_back(v);
    int p = par[v];
    if (p < 0) break;
    int e = edge_id[{v, p}];
    if (edges[e].erased) {
      edges[e].cost = 1;
      ++num_erased;
    }
  }
  reverse(ALL(path));
  if (num_erased == 0) {
    return nullopt;
  }
  i64 tbf = L - res1[t];
  assert(path.front() == s);
  int cur = s;
  i64 inc = 0;
  for (int i = 1; tbf > 0 and i < ssize(path); ++i) {
    int v = path[i];
    int e = edge_id[{cur, v}];
    if (edges[e].erased) {
      if (cap[v] - inc < 0) return nullopt;
      i64 delta = min(cap[v] - inc, tbf);
      edges[e].cost += delta;
      inc += delta;
      tbf -= delta;
    }
    cur = v;
  }
  if (tbf > 0) return nullopt;
  for (auto &e : edges) {
    if (e.cost == 0) e.cost = 1e18;
  }
  return edges;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  auto res = solve();
  if (not res.has_value()) {
    cout << "NO\n";
  } else {
    cout << "YES\n";
    for (auto &e : res.value()) {
      cout << e.from << ' ' << e.to << ' ' << e.cost << '\n';
    }
  }
}