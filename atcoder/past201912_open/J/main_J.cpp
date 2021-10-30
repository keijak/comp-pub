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

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
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
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
#define cerr if(false)std::cerr
#endif

using namespace std;

const Int kBig = 1e16;

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;


struct GridState {
  Int cost;
  int r;
  int c;
};
bool operator>(const GridState &x, const GridState &y) {
  return x.cost > y.cost;
}

auto solve() {
  int H = in, W = in;
  auto a = vector(H, vector(W, 0LL));  // cost
  REP(i, H) REP(j, W) a[i][j] = Int(in);

  auto search_shortest_path_on_grid = [&](const pair<int, int> &start) {
    static const int dx[4] = {1, 0, -1, 0};
    static const int dy[4] = {0, 1, 0, -1};
    auto inside = [&](int i, int j) {
      return 0 <= i and i < H and 0 <= j and j < W;
    };
    auto mincost = vector(H, vector(W, (Int) kBig));
    MinHeap<GridState> que;
    auto push = [&](Int cost, int r, int c) -> bool {
      if (not chmin(mincost[r][c], cost)) return false;
      que.push(GridState{cost, r, c});
      return true;
    };
    check(push(0LL, start.first, start.second));

    while (not que.empty()) {
      GridState cur = que.top();
      que.pop();
      if (cur.cost != mincost[cur.r][cur.c]) continue;
      for (int i = 0; i < 4; ++i) {
        const int nr = cur.r + dx[i];
        const int nc = cur.c + dy[i];
        if (not inside(nr, nc)) continue;
        const auto new_cost = cur.cost + a[nr][nc];
        push(new_cost, nr, nc);
      }
    }
    return mincost;
  };
  auto mc1 = search_shortest_path_on_grid({H - 1, 0});
  auto mc2 = search_shortest_path_on_grid({H - 1, W - 1});
  auto mc3 = search_shortest_path_on_grid({0, W - 1});
  Int ans = kBig;
  REP(i, H) {
    REP(j, W) {
      Int d = mc1[i][j] + mc2[i][j] + mc3[i][j] - a[i][j] * 2;
      chmin(ans, d);
    }
  }
  return ans;
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
