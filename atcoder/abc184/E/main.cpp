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

template <class T>
using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

struct Edge {
  i64 cost;
  int to;
};

struct GridState {
  i64 cost;
  int r;
  int c;
  //...
};
bool operator>(const GridState &x, const GridState &y) {
  return x.cost > y.cost;
}

std::vector<std::vector<std::optional<i64>>> grid_bfs(
    const std::vector<std::string> &g, const V<V<int>> &tele, int source_r,
    int source_c) {
  const int H = g.size();
  const int W = g[0].size();
  array<int, 4> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};

  V<int> warp_used(26);

  auto mincost = std::vector(H, std::vector(W, std::optional<i64>()));
  mincost[source_r][source_c] = 0LL;
  std::deque<GridState> que;
  que.push_back({0LL, source_r, source_c});
  while (not que.empty()) {
    GridState cur = std::move(que.front());
    que.pop_front();
    if (not mincost[cur.r][cur.c].has_value() or
        mincost[cur.r][cur.c].value() != cur.cost) {
      continue;
    }
    if (g[cur.r][cur.c] == 'G') break;
    for (int i = 0; i < 4; ++i) {
      int nr = cur.r + dy[i];
      int nc = cur.c + dx[i];
      if (nr < 0 or nr >= H or nc < 0 or nc >= W) continue;
      if (g[nr][nc] == '#') continue;
      i64 new_cost = cur.cost + 1;
      if (not mincost[nr][nc].has_value() or
          mincost[nr][nc].value() > new_cost) {
        mincost[nr][nc] = new_cost;
        que.push_back({new_cost, nr, nc});
      }
    }
    if (islower(g[cur.r][cur.c])) {
      int x = g[cur.r][cur.c] - 'a';
      if (not warp_used[x]) {
        warp_used[x] = true;
        for (auto u : tele[x]) {
          int nr = u / W;
          int nc = u % W;
          if (nr == cur.r and nc == cur.c) continue;
          i64 new_cost = cur.cost + 1;
          if (not mincost[nr][nc].has_value() or
              mincost[nr][nc].value() > new_cost) {
            mincost[nr][nc] = new_cost;
            que.push_back({new_cost, nr, nc});
          }
        }
      }
    }
  }
  return mincost;
}

i64 solve() {
  int H, W;
  cin >> H >> W;
  V<string> grid(H);
  int si = -1, sj = -1, gi = -1, gj = -1;
  V<V<int>> tele(26);
  REP(i, H) {
    cin >> grid[i];
    REP(j, W) {
      if (grid[i][j] == 'S') {
        si = i;
        sj = j;
      }
      if (grid[i][j] == 'G') {
        gi = i;
        gj = j;
      }
      if (islower(grid[i][j])) {
        int x = grid[i][j] - 'a';
        tele[x].push_back(i * W + j);
      }
    }
  }
  assert(si >= 0 and gi >= 0);

  auto res = grid_bfs(grid, tele, si, sj);
  if (not res[gi][gj].has_value()) return -1;
  return res[gi][gj].value();
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cout << solve() << endl;
}
