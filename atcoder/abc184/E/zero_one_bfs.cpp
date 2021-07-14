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

struct Edge {
  i64 cost;
  int to;
};

struct GridState {
  i64 cost;
  int node;
};
bool operator>(const GridState &x, const GridState &y) {
  return x.cost > y.cost;
}

const i64 INF = 1e18;

std::vector<i64> grid_bfs(const std::vector<std::string> &g,
                          const V<V<int>> &tele, int source_r, int source_c) {
  const int H = g.size();
  const int W = g[0].size();
  array<int, 4> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
  const int N = H * W + 26;
  auto mincost = std::vector(N, INF);
  int init_node = source_r * W + source_c;
  mincost[init_node] = 0LL;
  queue<GridState> que;
  que.push({0LL, init_node});
  while (not que.empty()) {
    GridState cur = std::move(que.front());
    que.pop();
    if (mincost[cur.node] != cur.cost) {
      continue;
    }
    if (cur.node >= H * W) {
      int x = cur.node - H * W;
      for (auto u : tele[x]) {
        int nr = u / W;
        int nc = u % W;
        int nx = nr * W + nc;
        i64 new_cost = cur.cost;
        if (mincost[nx] > new_cost) {
          mincost[nx] = new_cost;
          que.push({new_cost, nx});
        }
      }
    } else {
      int cur_r = cur.node / W;
      int cur_c = cur.node % W;
      if (g[cur_r][cur_c] == 'G') break;

      for (int i = 0; i < 4; ++i) {
        int nr = cur_r + dy[i];
        int nc = cur_c + dx[i];
        int nx = nr * W + nc;
        if (nr < 0 or nr >= H or nc < 0 or nc >= W) continue;
        if (g[nr][nc] == '#') continue;
        i64 new_cost = cur.cost + 1;
        if (mincost[nx] > new_cost) {
          mincost[nx] = new_cost;
          que.push({new_cost, nx});
        }
      }
      if (islower(g[cur_r][cur_c])) {
        int t = H * W + g[cur_r][cur_c] - 'a';
        i64 new_cost = cur.cost + 1;
        if (mincost[t] > new_cost) {
          mincost[t] = new_cost;
          que.push({new_cost, t});
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
  auto ans = res[gi * W + gj];
  return (ans == INF) ? -1 : ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cout << solve() << endl;
}
