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

template <class T>
using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct State {
  i64 cost;
  i64 last_cost;
  int node;
};
bool operator>(const State &x, const State &y) {
  if (x.cost != y.cost) return x.cost > y.cost;
  return x.last_cost > y.last_cost;
}

const i64 INF = 1e18;

pair<vector<State>, vector<int>> dijkstra(
    const std::vector<std::vector<Edge>> &g, int source) {
  const int n = g.size();
  std::vector<State> mincost(n);
  REP(i, n) { mincost[i] = {INF, INF, i}; }
  vector<int> pre(n, -1);
  mincost[source] = {0LL, 0LL, source};
  MinHeap<State> que;
  que.push(mincost[source]);
  while (not que.empty()) {
    State cur = std::move(que.top());
    que.pop();
    if (cur > mincost[cur.node]) {
      continue;
    }
    for (const auto &e : g[cur.node]) {
      i64 new_cost = cur.cost + e.cost;
      State new_state = {new_cost, e.cost, e.to};
      if (mincost[e.to] > new_state) {
        mincost[e.to] = new_state;
        pre[e.to] = cur.node;
        que.push(move(new_state));
      }
    }
  }
  return {move(mincost), move(pre)};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  V<i64> edge_w(m);
  map<pair<int, int>, int> edge_id;
  V<V<Edge>> g(n);
  REP(i, m) {
    int u, v;
    i64 w;
    cin >> u >> v >> w;
    u--;
    v--;
    edge_w[i] = w;
    edge_id[{u, v}] = edge_id[{v, u}] = i;
    g[u].push_back({w, v});
    g[v].push_back({w, u});
  }
  int start_node;
  cin >> start_node;
  start_node--;

  auto [mincost, pre] = dijkstra(g, start_node);
  i64 total_weight = 0;
  V<int> ans_edge;
  REP(i, n) {
    if (i == start_node) continue;
    int p = pre[i];
    int e = edge_id[{i, p}];
    ans_edge.push_back(e + 1);
    total_weight += edge_w[e];
  }
  cout << total_weight << '\n';
  sort(ALL(ans_edge));
  pprint(ans_edge);
}
