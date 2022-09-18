// #define NDEBUG
#include <bits/stdc++.h>
#include <atcoder/maxflow>

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

// T: flow capacity type (int or long long)
template<typename T>
struct ProjectSelection {
 public:
  // For each node v, make sure:
  // - sum(cost(*, v)) < kBigCost
  // - sum(cost(v, *)) < kBigCost
  static constexpr T kBigCost = std::numeric_limits<T>::max() / 10;

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
    if (cost == 0) return;
    assert(cost > 0);
    edges_.push_back(Edge{x, y, cost});
  }

  // {X=1} => cost
  void if_X_then_cost(int x, T cost) {
    if (cost < 0) return if_X_then_gain(x, -cost);
    if_X_and_notY_then_cost(x, kSink, cost);
  }

  // {X=0} => cost
  void if_notX_then_cost(int x, T cost) {
    if (cost < 0) return if_notX_then_gain(x, -cost);
    if_X_and_notY_then_cost(kSource, x, cost);
  }

  // {X=1} => gain
  void if_X_then_gain(int x, T gain) {
    if (gain < 0) return if_X_then_cost(x, -gain);
    bonus_ += gain;
    if_notX_then_cost(x, gain);
  }

  // {X=0} => gain
  void if_notX_then_gain(int x, T gain) {
    if (gain < 0) return if_notX_then_cost(x, -gain);
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
  int H = in, W = in;
  auto A = vector(H, vector(W, 0LL));
  REP(i, H) {
    REP(j, W) {
      cin >> A[i][j];
    }
  }
  const Int bigcost = 1e14;
  ProjectSelection<Int> psp(H + W);
  REP(i, H) {
    Int gain = 0;
    REP(j, W) gain += A[i][j];
    psp.if_X_then_gain(i, gain);
  }
  REP(j, W) {
    Int gain = 0;
    REP(i, H) gain += A[i][j];
    psp.if_notX_then_gain(H + j, gain);
  }
  REP(i, H) {
    const int row = i;
    REP(j, W) {
      const int col = H + j;
      const Int cost = (A[i][j] < 0) ? bigcost : A[i][j];
      psp.if_X_and_notY_then_cost(row, col, cost);
    }
  }
  auto ans = psp.max_gain();
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