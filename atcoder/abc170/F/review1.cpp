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

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

template<class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}
using Cost = Int;

struct GridState {
  Cost cost;
  int r;
  int c;
  int d;
};
bool operator>(const GridState &x, const GridState &y) {
  return x.cost > y.cost;
}

auto solve() -> Int {
  int H = in, W = in, K = in;
  int x1 = in, y1 = in, x2 = in, y2 = in;
  --x1, --y1, --x2, --y2;
  vector<string> g(H);
  REP(i, H) g[i] = string(in);
  const Cost kBigCost = (Int) kBig;

  auto res = [&]() {
    static const int dx[4] = {1, 0, -1, 0};
    static const int dy[4] = {0, 1, 0, -1};
    auto inside = [&](int x, int y) {
      return 0 <= x and x < H and 0 <= y and y < W;
    };
    auto mincost = vector(H, vector(W, vector(5, kBigCost)));
    MinHeap<GridState> que;
    auto push = [&](const Cost &cost, int r, int c, int d) -> bool {
      if (not inside(r, c)) return false;
      if (g[r][c] == '@') return false;
      if (not chmin(mincost[r][c][d], cost)) return false;
      que.push(GridState{cost, r, c, d});
      return true;
    };
    push(Cost{0}, x1, y1, 4);
    while (not que.empty()) {
      GridState cur = que.top();
      que.pop();
      if (cur.cost != mincost[cur.r][cur.c][cur.d]) continue;
      if (cur.r == x2 and cur.c == y2) return cur.cost;
      for (int i = 0; i < 4; ++i) {
        const int nr = cur.r + dx[i];
        const int nc = cur.c + dy[i];
        Cost new_cost = cur.cost;
        if (i != cur.d) {
          new_cost = ceil_div<Cost>(new_cost, K) * K;
        }
        new_cost++;
        push(new_cost, nr, nc, i);
      }
    }
    return *min_element(ALL(mincost[x2][y2]));
  }();
  if (res == kBigCost) return -1;
  return ceil_div<Cost>(res, K);
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
