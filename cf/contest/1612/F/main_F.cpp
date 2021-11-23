#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
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

struct State {
  int cost;
  int x, y;
  bool upx;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

auto solve() {
  int n = in, m = in, Q = in;
  set<pair<int, int>> special;
  REP(i, Q) {
    int a = in, b = in;
    special.emplace(a, b);
  }

  map<tuple<int, int, bool>, int> mincost;
  queue<State> que;

  auto push = [&](int cost, int x, int y, bool upx) -> bool {
    auto it = mincost.find({x, y, upx});
    if (it == mincost.end() or cost < it->second) {
      mincost[{x, y, upx}] = cost;
      que.push(State{cost, x, y, upx});
      return true;
    } else {
      return false;
    }
  };
  assert(push(0, 1, 1, true));
  assert(push(0, 1, 1, false));

  while (not que.empty()) {
    State cur = move(que.front());
    que.pop();
    if (cur.cost != mincost.at({cur.x, cur.y, cur.upx})) continue;
    if (cur.x == n and cur.y == m) return cur.cost;
    int p = cur.x + cur.y;
    if (special.count({cur.x, cur.y})) {
      p += 1;
    }
    if (cur.upx) {
      int nx = min(p, n);
      push(cur.cost + 1, nx, cur.y, false);
      if (nx - 1 > cur.x) {
        push(cur.cost + 1, nx - 1, cur.y, false);
      }
    } else {
      int ny = min(p, m);
      push(cur.cost + 1, cur.x, ny, true);
      if (ny - 1 > cur.y) {
        push(cur.cost + 1, cur.x, ny - 1, true);
      }
    }
  }
  check(false);
  return -1;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
