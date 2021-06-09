#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
using Float = long double;

#include <boost/multiprecision/cpp_int.hpp>
using i128 = boost::multiprecision::int128_t;
using i256 = boost::multiprecision::int256_t;
using i512 = boost::multiprecision::int512_t;
using i1024 = boost::multiprecision::int1024_t;
// using Int = boost::multiprecision::cpp_int;
// using Rational = boost::multiprecision::cpp_rational;

template <int sign = 1>
struct Infinity {
  template <typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned<T>::value,
                  "must be positive in an unsigned type");
    return (std::numeric_limits<T>::max() / T(2)) * T(sign);
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template <typename T>
  friend bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template <typename T>
  friend bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
const Infinity<> INF;

// Returns floor(sqrt(x)).
i64 floor_sqrt(i64 x) {
  assert(x >= 0);
  if (x <= 1) return x;
  i64 r = std::floor(sqrtl((long double)x));
  while (r * r < x) ++r;
  while (r * r > x) --r;
  return r;
}

template <class F, class T = std::invoke_result_t<F, Float>>
Float find_min_ternary_search(Float low, Float high, F f) {
  static_assert(std::is_invocable_v<F, Float>);
  std::less<T> compare;
  --low;  // Make it an open interval: (low, high).
  Float l = low, r = high;
  REP(iter, 80) {
    auto ll = (l + l + r) / 3.0;
    auto rr = (l + r + r) / 3.0;
    if (compare(f(ll), f(rr))) {
      r = rr;
    } else {
      l = ll;
    }
  }
  return (l + r) / 2.0;
}

using Int = boost::multiprecision::int128_t;
const Int BIG = INF;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  i64 c;
  i64 d;
  int to;
};

struct State {
  Int cost;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the source node to each node (if exists).
Int search(const vector<vector<Edge>> &g, int source, int goal) {
  const int n = g.size();
  auto mincost = vector(n, (Int)BIG);
  MinHeap<State> que;
  auto push = [&](Int cost, int node) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push({cost, node});
      return true;
    }
    return false;
  };
  assert(push(0, source));

  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    if (cur.cost > mincost[cur.node]) continue;
    if (cur.node == goal) break;
    for (const auto &e : g[cur.node]) {
      auto f = [&](Int w) -> Int {
        return cur.cost + w + e.c + e.d / (cur.cost + w + 1);
      };
      auto ff = [&](Float w) -> Float {
        return Float(cur.cost) + w + Float(e.c) +
               Float(e.d) / (Float(cur.cost) + Float(w) + 1);
      };
      Int new_cost = f(0);  // no wait.
      Int wmax = e.d / (cur.cost + 1);
      Int xm = (Int)find_min_ternary_search(0, Float(wmax + 1), ff);
      if (xm > 0) {
        chmin(new_cost, f(xm));
      }
      if (xm + 1 > 0) {
        chmin(new_cost, f(xm + 1));
      }
      push(new_cost, e.to);
    }
  }
  return mincost[goal];
}

Int solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<Edge>> g(n);
  REP(i, m) {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    --a, --b;
    g[a].push_back({c, d, b});
    g[b].push_back({c, d, a});
  }
  auto ans = search(g, 0, n - 1);
  return (ans == BIG) ? -1 : ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
