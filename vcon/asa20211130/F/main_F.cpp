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

using Graph = std::vector<std::set<int>>;

struct State {
  Int cost;
  int node;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the start node to each node (if exists).
auto search_shortest_path(const Graph &g, int start, int goal) {
  const int n = g.size();
  auto mincost = vector(n, (Int) kBig);
  auto parent = vector(n, -1);
  queue<State> que;
  auto push = [&](Int cost, int node, int from) -> bool {
    if (chmin(mincost[node], cost)) {
      parent[node] = from;
      que.push(State{cost, node});
      return true;
    }
    return false;
  };
  assert(push(0LL, start, -1));

  while (not que.empty()) {
    State cur = move(que.front());
    que.pop();
    if (cur.cost != mincost[cur.node]) continue;
    if (cur.node == goal) break;
    for (const auto &e: g[cur.node]) {
      auto new_cost = cur.cost + 1;
      push(new_cost, e, cur.node);
    }
  }
  return pair{mincost[goal], parent};
}

auto solve() {
  int n = in, m = in;
  vector<set<int>> g(n);
  vector<pair<int, int>> edges(m);
  map<pair<int, int>, int> eid;
  REP(i, m) {
    int s = in, t = in;
    --s, --t;
    edges[i] = {s, t};
    eid[{s, t}] = i;
    g[s].insert(t);
  }
  auto[dbase, parent] = search_shortest_path(g, 0, n - 1);
  DUMP(dbase);
  DUMP(parent);
  int node = n - 1;
  vector<Int> ans(m, dbase == kBig ? -1 : dbase);
  if (dbase == kBig) return ans;
  while (node != 0) {
    int p = parent[node];
    check(p >= 0);
    g[p].erase(node);
    auto[dx, _] = search_shortest_path(g, 0, n - 1);
    int e = eid.at({p, node});
    ans[e] = dx == kBig ? -1 : dx;
    g[p].insert(node);
    node = p;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print_seq(ans, "\n");
  }
}
