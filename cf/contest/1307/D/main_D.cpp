// #define NDEBUG
#include <bits/stdc++.h>
#include <ranges>

using Int = long long;

auto rep(int n) { return std::views::iota(0, std::max(n, 0)); };
template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

struct UnionFind {
  int n_;
  std::vector<int> parent_;  // negative: size

  explicit UnionFind(int n) : n_(n), parent_(n, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int v) {
    if (parent_[v] < 0) return v;
    return parent_[v] = find(parent_[v]);
  }

  int size(int v) { return -parent_[find(v)]; }

  bool same(int x, int y) { return find(x) == find(y); }
};

using namespace std;

//template<class T>
//using MinHeap = std::priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  int to;
};
using Graph = std::vector<std::vector<Edge>>;

struct State {
  int node;
  int cost;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

// Returns min distance from the start node to each node (if exists).
auto search_shortest_path(const Graph &g, int start) {
  const int n = g.size();
  auto mincost = vector(n, kBigVal<int>);
  deque<State> que;
  auto push = [&](int node, int cost) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push_back(State{node, cost});
      return true;
    }
    return false;
  };
  push(start, 0LL);

  while (not que.empty()) {
    State cur = que.front();
    que.pop_front();
    if (cur.cost != mincost[cur.node]) continue;
    for (const auto &e: g[cur.node]) {
      auto new_cost = cur.cost + 1;
      push(e.to, new_cost);
    }
  }
  return mincost;
}

auto solve() -> Int {
  const int n = in, m = in, K = in;
  vector<int> a = in(K); // special nodes, distinct
  for (auto &e: a) --e;

  Graph g(n);
  for (int i: rep(m)) {
    int x = in, y = in;
    --x, --y;
    g[x].push_back({y});
    g[y].push_back({x});
  }
  auto ds = search_shortest_path(g, 0);
  auto dg = search_shortest_path(g, n - 1);
  Int shortest = ds[n - 1];

  vector<array<int, 3>> dd(K);
  for (int i: rep(K)) {
    int x = a[i];
    dd[i] = {ds[x], dg[x], i};
  }
  ranges::sort(dd);
  int ans = -1;
  int maxds = -1;
  for (int i: rep(K)) {
    if (maxds != -1) {
      chmax(ans, maxds + 1 + dd[i][1]);
    }
    chmax(maxds, dd[i][0]);
  }
  chmin(ans, shortest);
  return ans;
}

int main() {
  init_();
  const int T = 1;//in;
  for (int t: rep(T)) {
    test_case(t, T);
    out(solve());
  }
  exit_();
}
