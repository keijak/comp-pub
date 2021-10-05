#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
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

struct Edge {
  i64 cost;
  int to;
};

struct State {
  pair<i64, i64> cost;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

auto solve() {
  int n = in, m = in;
  vector<i64> a = in(n);
  vector<vector<Edge>> g(n);
  REP(i, m) {
    i64 u = in, v = in, t = in;
    --u, --v;
    g[u].push_back(Edge{t, (int) v});
    g[v].push_back(Edge{t, (int) u});
  }

  auto search_shortest_path = [&](const vector<vector<Edge>> &g, int start, int goal) {
    const int n = g.size();
    auto mincost = vector(n, pair<i64, i64>{kBig, kBig});
    MinHeap<State> que;
    auto push = [&](pair<i64, i64> cost, int node) -> bool {
      if (chmin(mincost[node], cost)) {
        que.push(State{cost, node});
        return true;
      }
      return false;
    };
    assert(push(pair{0LL, -a[start]}, start));

    while (not que.empty()) {
      State cur = que.top();
      que.pop();
      if (cur.cost != mincost[cur.node]) continue;
      if (cur.node == goal) break;
      for (const auto &e: g[cur.node]) {
        auto[cur_t, cur_a] = cur.cost;
        auto new_cost = pair<i64, i64>{cur_t + e.cost, cur_a - a[e.to]};
        push(new_cost, e.to);
      }
    }
    return mincost[goal];
  };
  auto res = search_shortest_path(g, 0, n - 1);
  return -res.second;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
