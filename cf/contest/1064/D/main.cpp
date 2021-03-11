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

struct GridState {
  i64 cost;
  int r;
  int c;
};
bool operator>(const GridState &x, const GridState &y) {
  return x.cost > y.cost;
}

auto grid_search(const vector<string> &g, int source_r, int source_c) {
  const int H = g.size();
  const int W = g[0].size();
  array<int, 4> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
  vector mincost(H, vector(W, BIG));
  MinHeap<GridState> que;
  auto push = [&](i64 cost, int r, int c) -> bool {
    if (r < 0 or r >= H or c < 0 or c >= W) return false;
    if (g[r][c] == '*') return false;
    if (not chmin(mincost[r][c], cost)) return false;
    que.push({cost, r, c});
    return true;
  };
  assert(push(0LL, source_r, source_c));

  while (not que.empty()) {
    GridState cur = move(que.top());
    que.pop();
    if (cur.cost != mincost[cur.r][cur.c]) {
      continue;
    }
    assert(g[cur.r][cur.c] == '.');
    for (int i = 0; i < 4; ++i) {
      int nr = cur.r + dx[i];
      int nc = cur.c + dy[i];
      i64 new_cost = cur.cost;
      if (dy[i] == 1) {
        new_cost++;
      }
      push(new_cost, nr, nc);
    }
  }
  return mincost;
}

i64 solve() {
  int n, m;
  cin >> n >> m;
  int r, c;
  cin >> r >> c;
  --r, --c;
  i64 x, y;
  cin >> x >> y;
  vector<string> laby(n);
  REP(i, n) {
    string l;
    cin >> l;
    assert(ssize(l) == m);
    laby[i] = l;
  }
  auto mincost = grid_search(laby, r, c);
  i64 ans = 0;
  REP(i, n) {
    REP(j, m) {
      if (mincost[i][j] == BIG) {
        continue;
        // cerr << '_' << ' ';
      }
      //  cerr << mincost[i][j] << ' ';
      int yc = mincost[i][j];
      int xc = c + yc - j;
      if (0 <= yc and yc <= y and 0 <= xc and xc <= x) {
        ++ans;
      }
    }
    // cerr << endl;
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
