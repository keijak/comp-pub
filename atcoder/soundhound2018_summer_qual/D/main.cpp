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
template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

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

V<i64> dijkstra(const V<V<Edge>> &g, int source) {
  const int n = g.size();
  V<i64> mincost(n, INF);
  mincost[source] = 0;
  MinHeap<State> heap;
  heap.push({0LL, source});
  while (not heap.empty()) {
    State cur = move(heap.top());
    heap.pop();
    if (cur.cost != mincost[cur.node]) {
      continue;
    }
    for (const auto &e : g[cur.node]) {
      i64 new_cost = cur.cost + e.cost;
      if (mincost[e.to] > new_cost) {
        mincost[e.to] = new_cost;
        heap.push({new_cost, e.to});
      }
    }
  }
  return mincost;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m, s, t;
  cin >> n >> m >> s >> t;
  s--;
  t--;
  V<V<Edge>> ga(n), gb(n);
  REP(i, m) {
    int u, v, a, b;
    cin >> u >> v >> a >> b;
    u--;
    v--;
    ga[u].push_back({a, v});
    ga[v].push_back({a, u});
    gb[u].push_back({b, v});
    gb[v].push_back({b, u});
  }

  auto mincost_a = dijkstra(ga, s);
  auto mincost_b = dijkstra(gb, t);
  i64 mincost = mincost_a[n - 1] + mincost_b[n - 1];
  V<i64> ans(n);
  i64 init_val = 1e15;
  ans[n - 1] = init_val - mincost;
  for (int i = n - 2; i >= 0; --i) {
    chmin(mincost, mincost_a[i] + mincost_b[i]);
    ans[i] = init_val - mincost;
  }
  REP(i, n) { cout << ans[i] << '\n'; }
}
