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
  i64 weight;
  int to;
};

struct State {
  i64 cost;
  int city;
  int weight;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

int N;

// Returns min distance from the source node to each node (if exists).
vector<vector<i64>> dijkstra(const vector<vector<Edge>> &g, int source) {
  const int n = g.size();

  vector mincost(N, vector(51, INF));
  MinHeap<State> que;

  auto push = [&](int city, int weight, i64 cost) -> void {
    if (chmin(mincost[city][weight], cost)) {
      que.push({cost, city, weight});
    }
  };
  push(source, 0, 0);

  int done = 0;

  while (not que.empty()) {
    State cur = move(que.top());
    que.pop();
    if (cur.cost != mincost[cur.city][cur.weight]) continue;
    if (cur.weight == 0) {
      if (++done == N) break;
    }
    for (const auto &e : g[cur.city]) {
      i64 new_cost;
      i64 new_weight;
      if (cur.weight == 0) {
        new_cost = cur.cost;
        new_weight = e.weight;
      } else {
        i64 x = cur.weight + e.weight;
        new_cost = cur.cost + x * x;
        new_weight = 0;
      }
      push(e.to, new_weight, new_cost);
    }
  }
  return mincost;
}

vector<i64> solve() {
  int m;
  cin >> N >> m;
  vector<vector<Edge>> g(N);
  REP(i, m) {
    int v, u, w;
    cin >> v >> u >> w;
    --v, --u;
    g[v].push_back({w, u});
    g[u].push_back({w, v});
  }
  auto res = dijkstra(g, 0);
  vector<i64> ans(N);
  REP(i, N) {
    i64 x = res[i][0];
    ans[i] = (x == INF) ? -1 : x;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  print_seq(res);
}
