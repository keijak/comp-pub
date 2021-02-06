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
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
vector<i64> dijkstra(const vector<vector<Edge>> &g, int source) {
  const int n = g.size();
  vector<i64> mincost(n, INF);
  MinHeap<State> que;
  for (auto &e : g[source]) {
    chmin(mincost[e.to], e.cost);
    que.push({e.cost, e.to});
  }
  while (not que.empty()) {
    State cur = move(que.top());
    que.pop();
    if (cur.cost > mincost[cur.node]) continue;
    for (const auto &e : g[cur.node]) {
      auto new_cost = cur.cost + e.cost;
      if (chmin(mincost[e.to], new_cost)) {
        que.push({new_cost, e.to});
      }
    }
  }
  return mincost;
}

void solve() {
  int n, m;
  cin >> n >> m;
  vector<map<int, Edge>> gm(n);
  REP(i, m) {
    int a, b, c;
    cin >> a >> b >> c;
    --a, --b;
    auto it = gm[a].find(b);
    if (it == gm[a].end()) {
      gm[a][b] = {c, b};
    } else {
      if (c < it->second.cost) {
        gm[a][b] = {c, b};
      }
    }
  }
  vector<vector<Edge>> g(n);
  REP(i, n) {
    for (auto &[to, e] : gm[i]) {
      g[i].push_back(e);
    }
  }
  REP(i, n) {
    auto mindist = dijkstra(g, i);
    if (mindist[i] == INF) {
      cout << "-1\n";
    } else {
      cout << mindist[i] << "\n";
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
