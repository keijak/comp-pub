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

struct State {
  i64 cost;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
vector<i64> dijkstra(const vector<vector<int>> &g, int source) {
  const int n = g.size();
  vector<i64> mincost(n, INF);
  mincost[source] = 0LL;
  queue<State> que;
  que.push({0, source});
  while (not que.empty()) {
    State cur = move(que.front());
    que.pop();
    if (cur.cost > mincost[cur.node]) continue;
    for (const auto &e : g[cur.node]) {
      auto new_cost = cur.cost + 1;
      if (chmin(mincost[e], new_cost)) {
        que.push({new_cost, e});
      }
    }
  }
  return mincost;
}

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> g(n);
  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  int k;
  cin >> k;
  vector<int> c(k);
  REP(i, k) {
    cin >> c[i];
    --c[i];
  }
  if (k == 1) return 1;

  vector<vector<i64>> mincosts(k);
  REP(i, k) { mincosts[i] = dijkstra(g, c[i]); }

  vector dp(1 << k, vector(k, INF));
  REP(i, k) { dp[1 << i][i] = 1; }

  REP(bits, 1 << k) {
    vector<int> nx;
    nx.reserve(k);
    REP(i, k) {
      if (bits & (1 << i)) {
        nx.push_back(i);
      }
    }
    if (ssize(nx) <= 1) continue;
    REP(i, ssize(nx)) {
      int ni = nx[i];
      unsigned prev = bits & ~(1 << ni);
      REP(j, ssize(nx)) {
        if (j == i) continue;
        int nj = nx[j];
        if (dp[prev][nj] == INF) continue;
        i64 step = mincosts[nj][c[ni]];
        if (step == INF) continue;
        chmin(dp[bits][ni], dp[prev][nj] + step);
      }
    }

    DEBUG(nx, dp[bits]);
  }
  i64 ans = INF;
  unsigned full = (1 << k) - 1;
  REP(i, k) { chmin(ans, dp[full][i]); }
  return (ans == INF) ? -1 : ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
