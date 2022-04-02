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
void exit() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

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

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(std::move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }
};

auto solve() {
  Int n = in, m = in;
  vector<array<Int, 3>> edges(m);
  vector<Int> nodes;
  nodes.push_back(0);
  nodes.push_back(n - 1);
  REP(i, m) {
    Int a = in, b = in, c = in;
    --a, --b;
    edges[i] = {a, b, c};
    nodes.push_back(a);
    nodes.push_back(b);
  }
  Compressed<Int> cnodes(nodes);
  DUMP(cnodes.values);
  vector<map<int, Int>> to(cnodes.size());
  REP(i, m) {
    auto[a, b, c] = edges[i];
    int bi = cnodes.index(b);
    int ai = cnodes.index(a);
    if (to[bi].count(ai)) {
      chmin(to[bi][ai], c);
    } else {
      to[bi][ai] = c;
    }
    if (to[ai].count(bi)) {
      chmin(to[ai][bi], c);
    } else {
      to[ai][bi] = c;
    }
  }

  const int cn = cnodes.size();
  auto mincost = vector(cn, kBigVal<Int>);
  MinHeap<State> que;
  auto push = [&](Int cost, int node) -> bool {
    if (chmin(mincost[node], cost)) {
      que.push(State{node, cost});
      return true;
    }
    return false;
  };
  push(0LL, 0);

  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    if (cur.cost != mincost[cur.node]) continue;
    if (cur.node == cn - 1) break;
    for (const auto[v, el_cost]: to[cur.node]) {
      auto new_cost = cur.cost + el_cost;
      push(new_cost, v);
    }
    Int cx = cnodes.value(cur.node);
    if (cur.node + 1 < cn) {
      Int nx = cnodes.value(cur.node + 1);
      push(cur.cost + (nx - cx), cur.node + 1);
    }
    if (cur.node - 1 >= 0) {
      Int nx = cnodes.value(cur.node - 1);
      push(cur.cost + (cx - nx), cur.node - 1);
    }
  }
  print(mincost[cn - 1]);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit();
}
