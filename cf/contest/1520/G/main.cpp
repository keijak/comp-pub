#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
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
  return print_seq(a, ", ", "", (os << "{")) << "}";
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

const i64 BIG = 1e16;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct GridState {
  i64 cost;
  int r;
  int c;
};
bool operator>(const GridState &x, const GridState &y) {
  return x.cost > y.cost;
}

int n, m;
i64 walk_cost;

i64 grid_search1(const vector<vector<i64>> &g, int goal_r, int goal_c) {
  array<int, 4> dx = {0, 1, 0, -1}, dy = {1, 0, -1, 0};
  vector mincost(n * m, BIG);

  MinHeap<GridState> que;
  auto push = [&](i64 cost, int r, int c) -> bool {
    if (r < 0 or r >= n or c < 0 or c >= m) return false;
    if (g[r][c] == -1) return false;
    const int node = r * m + c;
    if (not chmin(mincost[node], cost)) return false;
    que.push({cost, r, c});
    return true;
  };
  assert(push(0LL, 0, 0));

  while (not que.empty()) {
    GridState cur = move(que.top());
    que.pop();
    const int cur_node = cur.r * m + cur.c;
    if (cur.cost != mincost[cur_node]) continue;
    const int r = cur.r;
    const int c = cur.c;
    if (r == goal_r and c == goal_c) break;

    // Go to a neighbor.
    for (int i = 0; i < 4; ++i) {
      const int nr = r + dx[i];
      const int nc = c + dy[i];
      const auto new_cost = cur.cost + walk_cost;
      push(new_cost, nr, nc);
    }
  }
  return mincost[goal_r * m + goal_c];
}

i64 grid_search2(const vector<vector<i64>> &g, int source_r, int source_c) {
  array<int, 4> dx = {0, 1, 0, -1}, dy = {1, 0, -1, 0};
  vector mincost(n * m + 1, BIG);
  const int kPortal = n * m;
  vector<int> portals;
  REP(i, n) REP(j, m) {
    if (g[i][j] > 0) {
      portals.push_back(i * m + j);
    }
  }

  MinHeap<GridState> que;
  auto push = [&](i64 cost, int r, int c) -> bool {
    if (r < 0 or r >= n or c < 0 or c >= m) return false;
    if (g[r][c] == -1) return false;
    const int node = r * m + c;
    if (not chmin(mincost[node], cost)) return false;
    que.push({cost, r, c});
    return true;
  };
  auto push_portal = [&](i64 cost) -> bool {
    if (not chmin(mincost[kPortal], cost)) return false;
    que.push({cost, n, 0});
    return true;
  };
  assert(push(0LL, source_r, source_c));

  while (not que.empty()) {
    GridState cur = move(que.top());
    que.pop();
    const int cur_node = cur.r * m + cur.c;
    if (cur.cost != mincost[cur_node]) continue;
    const bool is_portal = cur_node == kPortal;

    if (is_portal) break;

    const int r = cur.r;
    const int c = cur.c;

    // Go to a neighbor.
    for (int i = 0; i < 4; ++i) {
      const int nr = r + dx[i];
      const int nc = c + dy[i];
      const auto new_cost = cur.cost + walk_cost;
      push(new_cost, nr, nc);
    }

    // Warp
    if (g[r][c] > 0) {
      i64 new_cost = cur.cost + g[r][c];
      push_portal(new_cost);
    }
  }
  return mincost[kPortal];
}

i64 solve() {
  cin >> n >> m >> walk_cost;
  vector a(n, vector(m, 0LL));
  REP(i, n) REP(j, m) cin >> a[i][j];
  auto ans = grid_search1(a, n - 1, m - 1);  // no warp
  auto ans2 = grid_search2(a, 0, 0) + grid_search2(a, n - 1, m - 1);
  chmin(ans, ans2);
  return (ans == BIG ? -1 : ans);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
