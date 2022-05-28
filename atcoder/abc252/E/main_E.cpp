#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    assert(std::cin >> x);
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) assert(std::cin >> x);
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

void exit_() {
  std::string unused;
  assert(not(std::cin >> unused));
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<class T>
using MinHeap = std::priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  int to;
  Int cost;
};
using Graph = std::vector<std::vector<Edge>>;

struct State {
  int node;
  Int cost;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

auto solve() {
  int n = in, m = in;
  vector<vector<Edge>> g(n);
  map<pair<int, int>, int> edge_id;
  REP(i, m) {
    int a = in, b = in;
    Int c = in;
    --a, --b;
    g[a].push_back(Edge{b, c});
    g[b].push_back(Edge{a, c});
    edge_id[{a, b}] = edge_id[{b, a}] = i;
  }

  auto mincost = vector(n, kBigVal<Int>);
  vector<int> par(n, -1);
  MinHeap<State> que;
  auto push = [&](int node, Int cost, int from) -> bool {
    if (chmin(mincost[node], cost)) {
      par[node] = from;
      que.push(State{node, cost});
      return true;
    }
    return false;
  };
  push(0, 0LL, -1);

  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    if (cur.cost != mincost[cur.node]) continue;
    for (const auto &e: g[cur.node]) {
      auto new_cost = cur.cost + e.cost;
      push(e.to, new_cost, cur.node);
    }
  }
  DUMP(mincost);
  DUMP(par);

  vector<int> ans;
  REP(v, n) {
    if (par[v] == -1) continue;
    int e = edge_id[{v, par[v]}];
    ans.push_back(e + 1);
  }
  print_seq(ans);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
