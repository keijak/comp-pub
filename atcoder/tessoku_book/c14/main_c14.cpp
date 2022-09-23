// #define NDEBUG
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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
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
  const int n = in, m = in;
  Graph g(n);
  REP(i, m) {
    int a = in, b = in, c = in;
    --a, --b;
    g[a].push_back(Edge{b, c});
    g[b].push_back(Edge{a, c});
  }
  const int start = 0, goal = n - 1;

  auto mincost = vector(n, kBig<Int>);
  MinHeap<State> que;
  auto push = [&](int node, Int cost) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push(State{node, cost});
      return true;
    }
    return false;
  };
  push(start, 0LL);
  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    if (cur.cost != mincost[cur.node]) continue;
    if (cur.node == goal) break;
    for (const auto &e: g[cur.node]) {
      auto new_cost = cur.cost + e.cost;
      push(e.to, new_cost);
    }
  }
  DUMP(mincost);
  vector<char> done(n);
  queue<int> q;
  q.push(n - 1);
  done[n - 1] = true;
  while (not q.empty()) {
    int v = q.front();
    q.pop();
    for (auto &e: g[v]) {
      if (mincost[e.to] == mincost[v] - e.cost and not done[e.to]) {
        done[e.to] = true;
        q.push(e.to);
      }
    }
  }
  assert(done[0]);
  int ans = 0;
  REP(i, n) if (done[i]) ++ans;
  out(ans);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
