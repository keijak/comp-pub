#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <optional>
#include <queue>
#include <string>
#include <vector>
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

const i64 INF = 1e18;
int H, W;
char grid[105][105];
const array<int, 4> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
const array<char, 4> CMD = {'L', 'R', 'U', 'D'};

int pack(int r, int c) { return r * W + c; };
pair<int, int> unpack(int node) { return {node / W, node % W}; };

struct State {
  i64 cost;
  int node;
};

vector<vector<i64>> grid_search(int source_r, int source_c) {
  vector mincost(H, vector(W, INF));
  int source_node = pack(source_r, source_c);
  mincost[source_r][source_c] = 0;
  std::queue<State> que;
  que.push({0LL, source_node});
  while (not que.empty()) {
    State cur = std::move(que.front());
    que.pop();
    auto [cur_r, cur_c] = unpack(cur.node);
    if (mincost[cur_r][cur_c] != cur.cost) continue;
    if (grid[cur_r][cur_c] == 'F') break;
    for (int i = 0; i < 4; ++i) {
      int nr = cur_r + dx[i];
      int nc = cur_c + dy[i];
      if (nr < 0 or nr >= H or nc < 0 or nc >= W) continue;
      if (grid[nr][nc] == '*') continue;
      int new_node = pack(nr, nc);
      i64 new_cost = cur.cost + 1;
      if (mincost[nr][nc] > new_cost) {
        mincost[nr][nc] = new_cost;
        que.push({new_cost, new_node});
      }
    }
  }
  return mincost;
}

int main() {
  cin >> H >> W;
  int finish_r = -1, finish_c = -1;
  REP(i, H) {
    cin >> grid[i];
    REP(j, W) {
      if (grid[i][j] == 'F') {
        finish_r = i;
        finish_c = j;
      }
    }
  }
  assert(finish_r != -1);

  auto mincost = grid_search(0, 0);
  DEBUG(mincost);
  assert(mincost[finish_r][finish_c] < INF);

  vector<char> commands;
  {
    int i = finish_r, j = finish_c;
    while (i != 0 or j != 0) {
      i64 cur_cost = mincost[i][j];
      REP(d, 4) {
        int ni = i + dx[d];
        int nj = j + dy[d];
        if (ni < 0 or ni >= H or nj < 0 or nj >= W) continue;
        if (grid[ni][nj] == '*' or grid[ni][nj] == 'F') continue;
        if (mincost[ni][nj] != cur_cost - 1) continue;
        i = ni;
        j = nj;
        commands.push_back(CMD[d]);
        break;
      }
    }
    reverse(ALL(commands));
    DEBUG(commands);
  }

  // Play!
  {
    optional<bool> lr_swapped, ud_swapped;
    int i = 0, j = 0, L = ssize(commands);

    for (int cur = 0; cur < L; ++cur) {
      if (i == finish_r and j == finish_c) break;
      char c = commands[cur];
      DEBUG(cur, c);
      if (lr_swapped.value_or(false)) {
        if (c == 'L') {
          c = 'R';
        } else if (c == 'R') {
          c = 'L';
        }
      }
      if (ud_swapped.value_or(false)) {
        if (c == 'U') {
          c = 'D';
        } else if (c == 'D') {
          c = 'U';
        }
      }
      cout << c << endl;

      int res_i, res_j;
      cin >> res_i >> res_j;
      if (res_i == -1 and res_j == -1) {
        exit(0);  // something went wrong.
      }
      --res_i, --res_j;
      if (res_i == finish_r and res_j == finish_c) {
        exit(0);
      }
      if (c == 'R' and not lr_swapped.has_value()) {
        assert(i == res_i);
        if (res_j == j + 1) {
          lr_swapped = false;
        } else {
          assert(res_j == j);
          lr_swapped = true;
          --cur;
        }
      }
      if (c == 'D' and not ud_swapped.has_value()) {
        assert(j == res_j);
        if (res_i == i + 1) {
          ud_swapped = false;
        } else {
          assert(res_i == i);
          ud_swapped = true;
          --cur;
        }
      }
      i = res_i;
      j = res_j;
    }
  }
}
