#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
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
                        std::ostream *os = nullptr) {
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
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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

const i64 INF = 1e16;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct State {
  i64 cost;
  int node;
  unsigned flag;
};
bool operator>(const State &x, const State &y) {
  if (x.cost != y.cost) return x.cost > y.cost;
  return x.flag > y.flag;
}

// Returns min distance from the source node to each node (if exists).
vector<i64> dijkstra(const vector<vector<Edge>> &g, int source) {
  const int n = g.size();
  vector mincost(4, vector(n, INF));
  mincost[0][source] = 0LL;
  MinHeap<State> que;
  que.push({0, source, 0});
  while (not que.empty()) {
    State cur = move(que.top());
    que.pop();
    if (cur.cost > mincost[cur.flag][cur.node]) continue;
    for (const auto &e : g[cur.node]) {
      i64 cost_sum = cur.cost + e.cost;
      if (chmin(mincost[cur.flag][e.to], cost_sum)) {
        que.push({cost_sum, e.to, cur.flag});
      }
      if (not(cur.flag & 1)) {
        i64 next_cost = cost_sum - e.cost;
        unsigned next_state = cur.flag | 1;
        if (chmin(mincost[next_state][e.to], next_cost)) {
          que.push({next_cost, e.to, next_state});
        }
      }
      if (not(cur.flag & 2)) {
        i64 next_cost = cost_sum + e.cost;
        unsigned next_state = cur.flag | 2;
        if (chmin(mincost[next_state][e.to], next_cost)) {
          que.push({next_cost, e.to, next_state});
        }
      }
      if (not(cur.flag & 3)) {
        i64 next_cost = cost_sum;
        unsigned next_state = cur.flag | 3;
        if (chmin(mincost[next_state][e.to], next_cost)) {
          que.push({next_cost, e.to, next_state});
        }
      }
    }
  }
  return mincost[3];
}

void solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<Edge>> g(n);
  REP(i, m) {
    int v, u, w;
    cin >> v >> u >> w;
    --v, --u;
    g[u].push_back({w, v});
    g[v].push_back({w, u});
  }
  auto res = dijkstra(g, 0);
  for (int i = 1; i < n; ++i) {
    if (i > 1) cout << " ";
    cout << res[i];
  }
  cout << "\n";
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
