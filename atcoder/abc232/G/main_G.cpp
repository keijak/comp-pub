#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

const Int kBig = 1e17;

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  int to;
  Int cost;
};
using Graph = std::vector<std::vector<Edge>>;

struct State {
  Int cost;
  int node;
  int from;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

auto solve() {
  int n = in, M = in;
  vector<Int> A = in(n), B = in(n);
  set<pair<Int, int>> bmap;
  REP(i, n) {
    bmap.emplace(B[i], i);
  }
  const int kStart = 0, kGoal = n - 1;

  auto mincost = vector(n, (Int) kBig);
  mincost[kStart] = 0;
  MinHeap<State> que;
  que.push(State{0, kStart, -1});

  auto push = [&](int from) -> bool {
    auto it = bmap.lower_bound({M - A[from], -1});
    while (it == bmap.end() or it->second == from) {
      if (it == bmap.end()) {
        it = bmap.begin();
      } else {
        ++it;
      }
    }
    int to = it->second;
    Int cost = mincost[from] + (A[from] + it->first) % M;
    if (chmin(mincost[to], cost)) {
      que.push(State{cost, to, from});
      return true;
    }
    return false;
  };

  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    if (cur.node == kGoal) break;
    bmap.erase({B[cur.node], cur.node});
    if (cur.from != -1) {
      push(cur.from);
    }
    if (cur.cost != mincost[cur.node]) continue;
    push(cur.node);
  }
  return mincost[kGoal];
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
