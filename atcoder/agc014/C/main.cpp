#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
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

struct State {
  i64 magic;
  int steps;
  int closed;
  int node;
};
bool operator>(const State &x, const State &y) {
  if (x.magic != y.magic) return x.magic > y.magic;
  if (x.steps != y.steps) return x.steps < y.steps;
  return x.closed < y.closed;
}

i64 grid_search(const std::vector<std::string> &g, int source_r, int source_c,
                int K) {
  const int H = g.size();
  const int W = g[0].size();
  auto pack = [&](int r, int c) -> int { return r * W + c; };
  auto unpack = [&](int node) -> tuple<int, int> {
    return {node / W, node % W};
  };
  array<int, 4> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
  auto mincost = std::vector(H * W * 2, std::optional<State>());
  int source_node = pack(source_r, source_c);
  mincost[source_node] = {1, K, 0, source_node};
  MinHeap<State> que;
  que.push(mincost[source_node].value());
  while (not que.empty()) {
    State cur = std::move(que.top());
    que.pop();
    if (not mincost[cur.node].has_value() or cur > mincost[cur.node].value()) {
      continue;
    }
    auto [cur_r, cur_c] = unpack(cur.node);

    if (cur_r == 0 or cur_r == H - 1 or cur_c == 0 or cur_c == W - 1) {
      // REP(i, H) {
      //   REP(j, W) {
      //     int x = pack(i, j);
      //     if (mincost[x].has_value()) {
      //       cerr << ' ' << mincost[x].value().magic;
      //     } else {
      //       cerr << ' ' << '_';
      //     }
      //   }
      //   cerr << endl;
      // }
      return cur.magic;
    }

    for (int i = 0; i < 4; ++i) {
      int nr = cur_r + dy[i];
      int nc = cur_c + dx[i];
      char room = g[nr][nc];
      if (room == 'S') continue;
      int new_node = pack(nr, nc);
      State new_state;
      if (room == '.') {
        if (cur.steps > 0) {
          new_state = {cur.magic, cur.steps - 1, cur.closed, new_node};
        } else {
          new_state = {cur.magic + 1, K - 1, cur.closed + K, new_node};
        }
      } else {
        assert(room == '#');
        if (cur.steps > 0 and cur.closed > 0) {
          new_state = {cur.magic, cur.steps - 1, cur.closed - 1, new_node};
        } else {
          new_state = {cur.magic + 1, K - 1, cur.closed + K - 1, new_node};
        }
      }
      if (not mincost[new_node].has_value() or
          mincost[new_node].value() > new_state) {
        mincost[new_node] = new_state;
        que.push(new_state);
      }
    }
  }
  assert(false);
  return -1;
}

i64 solve() {
  int H, W, K;
  cin >> H >> W >> K;
  vector<string> grid(H);
  int si = -1, sj = -1;
  REP(i, H) {
    cin >> grid[i];
    REP(j, W) {
      if (grid[i][j] == 'S') {
        si = i;
        sj = j;
      }
    }
  }
  auto res = grid_search(grid, si, sj, K);
  return res;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
