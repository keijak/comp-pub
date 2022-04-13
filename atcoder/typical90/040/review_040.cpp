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
void exit_() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

#include <atcoder/maxflow>

constexpr Int kBigCost = std::numeric_limits<Int>::max() / 10;

// T: flow capacity type (int or long long)
template<typename T>
struct ProjectSelection {
 public:

  // Assign true to kSource and false to kSink.
  const int kSource, kSink;
  const int n_;  // number of initial nodes
  int m_;        // number of additional nodes

  struct Edge {
    int from;
    int to;
    T cost;
  };
  std::vector<Edge> edges_;

  T bonus_;

  std::optional<std::vector<bool>> assignment_;

  explicit ProjectSelection(int n)
      : kSource(n), kSink(n + 1), n_(n), m_(0), bonus_(0) {}

  // Returns the minimum cost.
  T min_cost() {
    atcoder::mf_graph<T> g(n_ + m_ + 2);
    for (const Edge &e: edges_) {
      g.add_edge(e.from, e.to, e.cost);
    }
    T res = g.flow(kSource, kSink) - bonus_;
    // Get the assignment.
    // assignment_ = g.min_cut(kSource);
    return res;
  }

  // Returns the maximum gain.
  T max_gain() { return -min_cost(); }

  // === constraint API ===

  void constraint(int x, int y, T cost) {
    if (x >= 0 and y < 0) {
      assert(cost >= 0);
      if_X_and_notY_then_cost(x, ~y, cost);
    } else if (x < 0 and y >= 0) {
      assert(cost >= 0);
      if_X_and_notY_then_cost(y, ~x, cost);
    } else if (x >= 0 and y >= 0) {
      assert(cost <= 0);
      if_all_of_X_then_gain({x, y}, -cost);
    } else if (x < 0 and y < 0) {
      assert(cost <= 0);
      if_none_of_X_then_gain({~x, ~y}, -cost);
    } else {
      assert(false);
    }
  }

  void constraint(int x, T cost) {
    if (x >= 0) {
      if (cost >= 0) {
        if_X_then_cost(x, cost);
      } else {
        if_X_then_gain(x, -cost);
      }
    } else {
      if (cost >= 0) {
        if_notX_then_cost(~x, cost);
      } else {
        if_notX_then_gain(~x, cost);
      }
    }
  }

  // === if-then API ===

  // {X=1, Y=0} => cost
  void if_X_and_notY_then_cost(int x, int y, T cost) {
    edges_.push_back(Edge{x, y, cost});
  }

  // {X=1} => cost
  void if_X_then_cost(int x, T cost) {
    if_X_and_notY_then_cost(x, kSink, cost);
  }

  // {X=0} => cost
  void if_notX_then_cost(int x, T cost) {
    if_X_and_notY_then_cost(kSource, x, cost);
  }

  // {X=1} => gain
  void if_X_then_gain(int x, T gain) {
    bonus_ += gain;
    if_notX_then_cost(x, gain);
  }

  // {X=0} => gain
  void if_notX_then_gain(int x, T gain) {
    bonus_ += gain;
    if_X_then_cost(x, gain);
  }

  // {X=0,Y=1} or {X=1,Y=0} => cost
  void if_X_not_equal_Y_then_cost(int x, int y, T cost) {
    if_X_and_notY_then_cost(x, y, cost);
    if_X_and_notY_then_cost(y, x, cost);
  }

  // {X=0,Y=0} or {X=1,Y=1} => gain
  void if_X_equal_Y_then_gain(int x, int y, T gain) {
    bonus_ += gain;
    if_X_and_notY_then_cost(x, y, gain);
    if_X_and_notY_then_cost(y, x, gain);
  }

  // {X[i]=1 forall i} => gain
  void if_all_of_X_then_gain(const std::vector<int> &xs, T gain) {
    const int y = n_ + 2 + m_++;
    bonus_ += gain;
    if_X_and_notY_then_cost(kSource, y, gain);
    for (const auto &x: xs) {
      if_X_and_notY_then_cost(y, x, kBigCost);
    }
  }

  // {X[i]=0 forall i} => gain
  void if_none_of_X_then_gain(const std::vector<int> &xs, T gain) {
    const int y = n_ + 2 + m_++;
    bonus_ += gain;
    if_X_and_notY_then_cost(y, kSink, gain);
    for (const auto &x: xs) {
      if_X_and_notY_then_cost(x, y, kBigCost);
    }
  }
};

auto solve() {
  const int n = in;
  const Int W = in;
  vector<Int> A = in(n); // revenue
  vector<vector<int>> deps(n);
  REP(i, n) {
    int k = in;
    REP(j, k) {
      int c = in;
      --c;
      deps[c].push_back(i);
    }
  }
  ProjectSelection<Int> psp(n);
  REP(i, n) {
    psp.if_X_then_gain(i, A[i]);
    psp.if_X_then_cost(i, W);
    for (int v: deps[i]) {
      psp.if_X_and_notY_then_cost(i, v, kBigCost);
    }
  }
  print(psp.max_gain());
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}
