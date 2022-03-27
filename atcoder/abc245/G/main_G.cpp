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
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
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


template<class T>
using MinHeap = std::priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  int to;
  Int cost;
};
using Graph = std::vector<std::vector<Edge>>;


// cost to country
using Cost = pair<Int, int>;

struct State {
  int node;
  Cost cost;
};
bool operator>(const State &x, const State &y) {
  return x.cost > y.cost;
}

auto solve() {
  int n = in, m = in, K = in, L = in;
  vector<vector<Edge>> to(n);
  vector<int> country = in(n);
  for (int &x: country) {
    --x;
    assert(x < K);
  }
  vector<int> popular(n);
  REP(i, L) {
    int j = in;
    --j;
    popular[j] = true;
  }
  REP(i, m) {
    int u = in, v = in;
    --u, --v;
    Int c = in;
    to[u].push_back(Edge{v, c});
    to[v].push_back(Edge{u, c});
  }

  auto mincost = vector(n, vector<Cost>());
  MinHeap<State> que;
  auto push = [&](const Cost &cost, int node) -> bool {
    auto &cur = mincost[node];
    if (ssize(cur) == 2 and cur[1] <= cost) {
      return false;
    }
    bool done = false;
    for (auto &co: cur) {
      if (co.second == cost.second) {
        if (co.first <= cost.first) return false;
        co = cost;
        done = true;
      }
    }
    if (not done) {
      cur.push_back(cost);
    }
    sort(ALL(cur));
    while (ssize(cur) > 2) cur.pop_back();
    que.push(State{node, cost});
    return true;
  };

  REP(v, n) {
    if (popular[v]) {
      for (auto &e: to[v]) {
        Cost co = {e.cost, country[v]};
        push(co, e.to);
      }
    }
  }

  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    {
      const auto &best = mincost[cur.node];
      bool ok = false;
      for (auto &co: best) {
        if (co == cur.cost) {
          ok = true;
          break;
        }
      }
      if (not ok) continue;
    }

    for (const auto &e: to[cur.node]) {
      Cost co = cur.cost;
      co.first += e.cost;
      push(co, e.to);
    }
  }

  vector<Int> ans(n);
  REP(v, n) {
    int mycountry = country[v];
    Int cost = kBigVal<Int>;
    DUMP(v, mycountry, mincost[v]);
    for (auto[cand, cc]: mincost[v]) {
      if (cc != -1 and cc != mycountry) {
        chmin(cost, cand);
      }
    }
    ans[v] = (cost == kBigVal<Int>) ? -1LL : cost;
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    print_seq(solve());
  }
}
