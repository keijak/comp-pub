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

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

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

struct Edge {
  int to;
  i64 w;
};
bool operator<(const Edge &e1, const Edge &e2) {
  return e1.w < e2.w;
}
bool operator>(const Edge &e1, const Edge &e2) {
  return e1.w > e2.w;
}

struct BipartiteChecker {
 private:
  int n_;
  vector<vector<Edge>> g_;
  vector<int> depth_;
  vector<int> prev_;
  optional<deque<int>> odd_cycle_;

 public:
  explicit BipartiteChecker(vector<vector<Edge>> g) :
      n_(g.size()), g_(move(g)), depth_(n_, -1), prev_(n_, -1) {
    run();
  }

  bool is_bipartite() const { return not odd_cycle_.has_value(); }

  const deque<int> &odd_cycle() const { return odd_cycle_.value(); }

  int color(int v) const { return depth_[v] % 2; }

 private:
  void run() {
    for (int i = 0; i < n_; ++i) {
      if (depth_[i] != -1) continue;
      bfs(i);
      if (odd_cycle_.has_value()) break;
    }
  }

  void bfs(int start) {
    queue<int> q;
    depth_[start] = 0;
    q.push(start);
    while (not q.empty()) {
      int v = q.front();
      q.pop();
      const int d = depth_[v] + 1;
      for (const auto &e: g_[v]) {
        if (depth_[e.to] != -1) {
          if (depth_[e.to] % 2 == d % 2) continue;
          // Detected an odd cycle!
          deque<int> cycle;
          for (; v != -1 and depth_[v] > depth_[e.to]; v = prev_[v]) {
            cycle.push_front(v);
          }
          int u = e.to;
          while (v != u) {
            assert(v != -1 and u != -1);
            cycle.push_front(v);
            cycle.push_back(u);
            v = prev_[v];
            u = prev_[u];
          }
          cycle.push_front(v);
          odd_cycle_ = std::move(cycle);
          return;
        }
        depth_[e.to] = d;
        prev_[e.to] = v;
        q.push(e.to);
      }
    }
  }
};

auto solve() -> i64 {
  const int n = in, m = in;
  vector<vector<Edge>> g(n);
  map<pair<int, int>, i64> smap;
  REP(i, m) {
    const auto u = int(in) - 1, v = int(in) - 1;
    const i64 s = in;
    g[u].push_back(Edge{v, s});
    g[v].push_back(Edge{u, s});
    smap[{u, v}] = smap[{v, u}] = s;
  }
  vector<i64> T(n, 0);
  BipartiteChecker checker(g);
  if (checker.is_bipartite()) {
    vector<int> visited(n, 0);
    queue<int> q;
    visited[0] = true;
    T[0] = 1;
    q.push(0);
    while (not q.empty()) {
      int v = q.front();
      q.pop();
      for (const auto &e: g[v]) {
        if (visited[e.to]) continue;
        visited[e.to] = true;
        T[e.to] = e.w - T[v];
        q.push(e.to);
      }
    }
    for (const auto&[p, s]: smap) {
      auto[u, v] = p;
      if (T[u] + T[v] != s) return 0;
    }
    vector<i64> mins(2, i64(kBig));
    REP(v, n) {
      int c = checker.color(v);
      chmin(mins[c], T[v]);
    }
    DUMP(mins);
    return max(mins[0] + mins[1] - 1, 0LL);
  } else {
    const auto &cycle = checker.odd_cycle();
    DUMP(cycle);
    assert(ssize(cycle) % 2 == 1);
    assert(ssize(cycle) >= 3);
    T[cycle.front()] = 1;
    REP(i, 1, ssize(cycle)) {
      int u = cycle[i - 1], v = cycle[i];
      i64 s = smap[{u, v}];
      T[v] = s - T[u];
    }
    int u0 = cycle.front(), v0 = cycle.back();
    i64 s = smap[{u0, v0}];
    i64 tt = T[u0] + T[v0];
    if (abs(s - tt) % 2 == 1) return 0;
    i64 delta = abs(s - tt) / 2;
    if (s < tt) delta *= -1;
    vector<int> visited(n, 0);
    queue<int> q;
    REP(i, ssize(cycle)) {
      int v = cycle[i];
      if (i % 2 == 0) {
        T[v] += delta;
      } else {
        T[v] -= delta;
      }
      visited[v] = true;
      q.push(v);
    }
    while (not q.empty()) {
      int v = q.front();
      q.pop();
      for (const auto &e: g[v]) {
        if (visited[e.to]) continue;
        visited[e.to] = true;
        T[e.to] = e.w - T[v];
        q.push(e.to);
      }
    }
    REP(v, n) if (T[v] <= 0) return 0;
    for (const auto&[p, s]: smap) {
      auto[u, v] = p;
      if (T[u] + T[v] != s) return 0;
    }
    return 1;
  }
  assert(false);
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
