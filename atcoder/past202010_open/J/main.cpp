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

template <class T>
using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct State {
  i64 cost;
  int node;
  //...
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

const i64 INF = 1e18;
string warp;
V<set<int>> wnodes(3);
map<pair<char, char>, i64> warp_cost;

std::vector<i64> dijkstra(const std::vector<std::vector<Edge>> &g, int source) {
  const int n = g.size();
  std::vector<i64> mincost(n, INF);
  mincost[source] = 0LL;
  MinHeap<State> que;
  que.push({0LL, source});
  while (not que.empty()) {
    State cur = std::move(que.top());
    que.pop();
    if (cur.cost != mincost[cur.node]) {
      continue;
    }
    for (const auto &e : g[cur.node]) {
      i64 new_cost = cur.cost + e.cost;
      if (mincost[e.to] > new_cost) {
        mincost[e.to] = new_cost;
        que.push({new_cost, e.to});
      }
    }
  }
  return mincost;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  i64 Xab, Xac, Xbc;
  cin >> Xab >> Xac >> Xbc;
  warp_cost[{'A', 'B'}] = warp_cost[{'B', 'A'}] = Xab;
  warp_cost[{'A', 'C'}] = warp_cost[{'C', 'A'}] = Xac;
  warp_cost[{'B', 'C'}] = warp_cost[{'C', 'B'}] = Xbc;

  cin >> warp;
  REP(i, N) { wnodes[warp[i] - 'A'].insert(i); }

  auto calc_cost = [&](int i, int j, i64 base) -> i64 {
    i64 res = base;
    if (warp[i] != warp[j]) {
      chmin(res, warp_cost[pair(warp[i], warp[j])]);
    }
    return res;
  };

  V<V<Edge>> adj(N + 3);
  for (int i = 0; i < M; ++i) {
    int u, v;  // read as 1-indexed.
    i64 c;
    cin >> u >> v >> c;
    u--, v--;  // to 0-indexed.
    // c = calc_cost(u, v, c);
    adj[u].push_back({c, v});
    adj[v].push_back({c, u});
  }

  REP(i, N) {
    int wtype = warp[i] - 'A';
    adj[N + wtype].push_back({0LL, i});
    REP(j, 3) {
      if (j != wtype) {
        i64 c = warp_cost[{warp[i], 'A' + j}];
        adj[i].push_back({c, N + j});
      }
    }
  }

  //   REP(i, 3) {
  //     REP(j, 3) {
  //       if (i == j) continue;
  //       i64 c = warp_cost[{'A' + i, 'A' + j}];
  //       adj[N + i].push_back({c, N + j});
  //     }
  //   }

  auto res = dijkstra(adj, 0);
  DEBUG(res);
  cout << res[N - 1] << endl;
}
