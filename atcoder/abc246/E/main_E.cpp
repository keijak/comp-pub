#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
void exit() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct GridState {
  int r;
  int c;
  int dir;
  Int cost;
};
bool operator>(const GridState &x, const GridState &y) {
  return x.cost > y.cost;
}

auto solve() {
  int H = in;
  int W = H;
  int ax = in, ay = in, bx = in, by = in;
  --ax, --ay, --bx, --by;
  vector<string> g(H);
  REP(i, H) {
    cin >> g[i];
  }

  static const int dx[4] = {1, -1, 1, -1};
  static const int dy[4] = {1, -1, -1, 1};
  auto inside = [&](int i, int j) -> bool {
    return 0 <= i and i < H and 0 <= j and j < W;
  };
  auto mincost = vector(H, vector(W, vector(4, kBigVal<Int>)));
  deque<GridState> que;
  auto push = [&](Int cost, int r, int c, int d, bool pback) -> bool {
    if (not inside(r, c)) return false;
    if (g[r][c] == '#') return false;
    if (not chmin(mincost[r][c][d], cost)) return false;
    if (pback) {
      que.push_back(GridState{r, c, d, cost});
    } else {
      que.push_front(GridState{r, c, d, cost});
    }
    return true;
  };
  for (int i = 0; i < 4; ++i) {
    const int nx = ax + dx[i];
    const int ny = ay + dy[i];
    push(1LL, nx, ny, i, true);
  }
  pair<int, int> goal = {bx, by};

  while (not que.empty()) {
    GridState cur = que.front();
    que.pop_front();
    if (cur.cost != mincost[cur.r][cur.c][cur.dir]) continue;
    if (pair(cur.r, cur.c) == goal) break;
    assert(g[cur.r][cur.c] == '.');
    for (int i = 0; i < 4; ++i) {
      const int nr = cur.r + dx[i];
      const int nc = cur.c + dy[i];
      // Note: If the cost delta is always 0 or 1, BFS should be enough.
      const auto new_cost = cur.cost + ((i == cur.dir) ? 0 : 1);
      push(new_cost, nr, nc, i, (i != cur.dir));
    }
  }
  Int ans = kBigVal<Int>;
  REP(d, 4) {
    chmin(ans, mincost[bx][by][d]);
  }
  print(ans == kBigVal<Int> ? -1 : ans);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit();
}
