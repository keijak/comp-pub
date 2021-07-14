#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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
  int node;
  //...
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
std::vector<std::optional<i64>> dijkstra(
    const std::vector<std::vector<Edge>> &g, int source) {
  const int n = g.size();
  std::vector<std::optional<i64>> mincost(n);
  mincost[source] = 0LL;
  MinHeap<State> que;
  que.push({0LL, source});
  while (not que.empty()) {
    State cur = std::move(que.top());
    que.pop();
    if (not mincost[cur.node].has_value() or
        cur.cost != mincost[cur.node].value()) {
      continue;
    }
    for (const auto &e : g[cur.node]) {
      i64 new_cost = cur.cost + e.cost;
      if (not mincost[e.to].has_value() or mincost[e.to].value() > new_cost) {
        mincost[e.to] = new_cost;
        que.push({new_cost, e.to});
      }
    }
  }
  return mincost;
}

i64 solve() {
  i64 a, b, x, y;
  cin >> a >> b >> x >> y;
  --a, --b;
  vector<vector<Edge>> g(200);
  REP(i, 99) {
    g[i].push_back({y, i + 1});
    g[i + 1].push_back({y, i});
    g[100 + i].push_back({y, i + 101});
    g[i + 101].push_back({y, i + 100});
    g[i + 1].push_back({x, i + 100});
    g[i + 100].push_back({x, i + 1});
  }
  REP(i, 100) {
    g[i].push_back({x, i + 100});
    g[i + 100].push_back({x, i});
  }
  auto res = dijkstra(g, a);
  return res[b + 100].value();
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << solve() << endl;
}
