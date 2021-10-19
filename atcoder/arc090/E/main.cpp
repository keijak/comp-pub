#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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
} const in;

inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) throw std::runtime_error(message);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

// s &= t;
// Merges two sets into their intersection.
// The first argument will be updated to the result.
template<typename T>
std::set<T> &operator&=(std::set<T> &s, const std::set<T> &t) {
  auto it = s.begin();
  auto jt = t.begin();
  while (it != s.end()) {
    while (jt != t.end() and *jt < *it) ++jt;
    if (jt == t.end()) {
      s.erase(it, s.end());
      break;
    }
    if (*it < *jt) {
      it = s.erase(it);
    } else {
      ++it, ++jt;
    }
  }
  return s;
}
template<typename T>
std::set<T> &operator&=(std::set<T> &s, std::set<T> &&t) {
  if (s.size() <= t.size()) {
    const std::set<T> &c(t);
    s &= c;
  } else {
    const std::set<T> &c(s);
    t &= c;
    s.swap(t);
  }
  return s;
}

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

struct Edge {
  int to;
  Int w;
  int eid;
};

using Graph = vector<vector<Edge>>;


struct State {
  Int cost;
  int node;
};
bool operator>(const State &x, const State &y) { return tie(x.cost, x.node) > tie(y.cost, y.node); }

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

auto solve() {
  int n = in, m = in;
  int S = in, T = in;
  --S, --T;
  Graph g(n);
  vector<Int> edge_cost(m);
  REP(i, m) {
    int u = in, v = in, d = in;
    --u, --v;
    g[u].push_back({v, Int(d), i});
    g[v].push_back({u, Int(d), i});
    edge_cost[i] = d;
  }

  auto mincost = vector(n, (Int) kBig);
  auto counts = vector(n, Mint(0));
  auto edge_mincost = vector(m, pair<Int, Int>(kBig, kBig));
  auto edge_from = vector(m, -1);
  auto edge_to = vector(m, -1);
  auto mincost1 = vector(n, (Int) kBig);
  auto counts1 = vector(n, Mint(0));
  auto edge_mincost1 = vector(m, pair<Int, Int>(kBig, kBig));
  auto edge_from1 = vector(m, -1);
  auto edge_to1 = vector(m, -1);

  auto search_shortest_path = [&](const Graph &g, int start) {
    MinHeap<State> que;
    auto push = [&](Int cost, int node, int from) -> bool {
      Mint fc = (from == -1) ? 1 : counts[from];
      if (chmin(mincost[node], cost)) {
        counts[node] = fc;
        que.push(State{cost, node});
        return true;
      } else if (mincost[node] == cost) {
        counts[node] += fc;
      }
      return false;
    };
    check(push(0LL, start, -1));

    while (not que.empty()) {
      State cur = que.top();
      que.pop();
      if (cur.cost != mincost[cur.node]) continue;
      for (const auto &e: g[cur.node]) {
        auto new_cost = cur.cost + e.w;
        push(new_cost, e.to, cur.node);

        if (cur.cost < edge_mincost[e.eid].first) {
          edge_mincost[e.eid] = pair<Int, Int>{cur.cost, cur.cost + e.w};
          edge_from[e.eid] = cur.node;
          edge_to[e.eid] = e.to;
        }
      }
    }
  };
  search_shortest_path(g, S);
  Int spath_len = mincost[T];
  DUMP(spath_len);
  set<int> node_set1, edge_set1;
  if (spath_len % 2 == 0) {
    REP(v, n) {
      if (mincost[v] != kBig and mincost[v] * 2 == spath_len) {
        node_set1.insert(v);
      }
    }
  }
  REP(ei, m) {
    auto[l, r] = edge_mincost[ei];
    if (l != kBig and l * 2 < spath_len and spath_len < r * 2 and
        l == mincost[edge_from[ei]] and r == mincost[edge_to[ei]]) {
      edge_set1.insert(ei);
    }
  }

  mincost1.swap(mincost);
  counts1.swap(counts);
  edge_mincost1.swap(edge_mincost);
  edge_from1.swap(edge_from);
  edge_to1.swap(edge_to);
  search_shortest_path(g, T);
  check(spath_len == mincost[S]);
  check(counts1[T].val() == counts[S].val());

  set<int> node_set2, edge_set2;
  if (spath_len % 2 == 0) {
    REP(v, n) {
      if (mincost[v] != kBig and mincost[v] * 2 == spath_len) {
        node_set2.insert(v);
      }
    }
  }
  REP(ei, m) {
    auto[l, r] = edge_mincost[ei];
    if (l != kBig and l * 2 < spath_len and spath_len < r * 2 and
        l == mincost[edge_from[ei]] and r == mincost[edge_to[ei]]) {
      edge_set2.insert(ei);
    }
  }
  node_set1 &= node_set2;
  edge_set1 &= edge_set2;
  DUMP(node_set1);
  DUMP(edge_set1);
  Mint cans = 0;
  for (auto v: node_set1) {
    if (mincost1[v] + mincost[v] == spath_len) {
      cans += (counts1[v] * counts[v]).pow(2);
    }
  }
  for (auto e: edge_set1) {
    int u = edge_from1[e];
    int v = edge_from[e];
    if (u == v) continue;
    if (mincost1[u] + mincost[v] + edge_cost[e] == spath_len) {
      cans += (counts1[u] * counts[v]).pow(2);
    }
  }
  return counts1[T] * counts[S] - cans;
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
