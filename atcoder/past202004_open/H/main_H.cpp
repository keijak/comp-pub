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
  int k;
};
bool operator>(const GridState &x, const GridState &y) {
  return x.cost > y.cost;
}

auto solve() {
  int n = in, m = in;
  vector<string> g(n);
  REP(i, n) cin >> g[i];
  int si, sj, gi, gj;
  REP(i, n) REP(j, m) {
      if (g[i][j] == 'S') {
        si = i, sj = j;
      }
      if (g[i][j] == 'G') {
        gi = i, gj = j;
      }
    }
  DUMP(gi, gj);

  auto search_shortest_path_on_grid = [&]() {
    const int H = n;
    const int W = m;
    const array<int, 4> dx = {0, 1, 0, -1}, dy = {1, 0, -1, 0};
    auto mincost = vector(H, vector(W, vector(11, (Int) kBig)));
    queue<GridState> que;
    auto push = [&](Int cost, int r, int c, int k) -> bool {
      if (not chmin(mincost[r][c][k], cost)) return false;
      que.push(GridState{cost, r, c, k});
      return true;
    };
    assert(push(0LL, si, sj, 0));

    while (not que.empty()) {
      GridState cur = que.front();
      que.pop();
      if (cur.cost != mincost[cur.r][cur.c][cur.k]) continue;
      if (cur.r == gi and cur.c == gj and cur.k == 9) break;
      for (int i = 0; i < 4; ++i) {
        const int nr = cur.r + dx[i];
        const int nc = cur.c + dy[i];
        const auto new_cost = cur.cost + 1;
        if (nr < 0 or nr >= H or nc < 0 or nc >= W) continue;
        char ch = g[nr][nc];
        int nk = cur.k;
        if (isdigit(ch) and (ch - '0') == cur.k + 1) {
          nk = cur.k + 1;
        }
        push(new_cost, nr, nc, nk);
      }
    }
    return mincost[gi][gj][9];
  };
  auto res = search_shortest_path_on_grid();
  return (res == kBig) ? -1LL : res;
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