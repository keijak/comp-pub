#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "YES" : "NO") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

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

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<int sign = 1>
struct Infinity {
  template<typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      static_assert(std::numeric_limits<T>::max() != T());  // max must be defined
      return T(sign) * (std::numeric_limits<T>::max() / T(2));
    }
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template<typename T>
  friend constexpr bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template<typename T>
  friend constexpr bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
constexpr Infinity<> kBig;

struct GridState {
  Int cost;
  int r;
  int c;
};
bool operator>(const GridState &x, const GridState &y) {
  return x.cost > y.cost;
}

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

auto search_shortest_path_on_grid(const vector<string> &g,
                                  const pair<int, int> &start,
                                  const pair<int, int> &goal) {
  static const int dx[4] = {1, 0, -1, 0};
  static const int dy[4] = {0, 1, 0, -1};
  const int H = g.size();
  const int W = g[0].size();
  auto inside = [&](int i, int j) -> bool {
    return 0 <= i and i < H and 0 <= j and j < W;
  };
  auto mincost = vector(H, vector(W, (Int) kBig));
  MinHeap<GridState> que;
  auto push = [&](Int cost, int r, int c) -> bool {
    if (not chmin(mincost[r][c], cost)) return false;
    que.push(GridState{cost, r, c});
    return true;
  };
  assert(push(0LL, start.first, start.second));

  while (not que.empty()) {
    GridState cur = que.top();
    que.pop();
    if (cur.cost != mincost[cur.r][cur.c]) continue;
    if (pair(cur.r, cur.c) == goal) break;
    for (int i = 0; i < 4; ++i) {
      const int nr = cur.r + dx[i];
      const int nc = cur.c + dy[i];
      if (not inside(nr, nc)) continue;
      Int new_cost = cur.cost;
      if (g[nr][nc] == '#') {
        if (cur.cost == 2) continue;
        ++new_cost;
      }
      push(new_cost, nr, nc);
    }
  }
  return mincost[goal.first][goal.second];
}

auto solve() {
  int H = in, W = in;
  cin.ignore();
  vector<string> g(H);
  REP(i, H) cin >> g[i];
  pair<int, int> start, goal;
  REP(i, H) {
    DUMP(i, g[i]);
    assert((int) g[i].size() == int(W));
    DUMP(g[i]);
    REP(j, W) {
      if (g[i][j] == 's') {
        start = {i, j};
      }
      if (g[i][j] == 'g') {
        goal = {i, j};
      }
    }
  }
  auto res = search_shortest_path_on_grid(g, start, goal);
  return res <= 2;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
