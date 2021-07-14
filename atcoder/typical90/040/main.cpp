#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

#include <atcoder/maxflow>

const i64 BIG_COST = i64(2e9);

// T: flow capacity type (i32/i64)
template <typename T>
struct ProjectSelection {
 private:
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

 public:
  explicit ProjectSelection(int n)
      : kSource(n), kSink(n + 1), n_(n), m_(0), bonus_(0) {}

  // Returns the minimum cost.
  T min_cost() const {
    // Build the graph and compute max flow.
    atcoder::mf_graph<T> g(n_ + m_ + 2);
    for (const Edge &e : edges_) {
      g.add_edge(e.from, e.to, e.cost);
    }
    return g.flow(kSource, kSink) - bonus_;
  }

  // Returns the maximum gain.
  T max_gain() const { return -min_cost(); }

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

  // {Xi=1 forall i} => gain
  void if_all_of_X_then_gain(const std::vector<int> &xs, T gain) {
    const int y = n_ + 2 + m_++;
    bonus_ += gain;
    if_X_and_notY_then_cost(kSource, y, gain);
    for (const auto &x : xs) {
      if_X_and_notY_then_cost(y, x, BIG_COST);
    }
  }

  // {Xi=0 forall i} => gain
  void if_none_of_X_then_gain(const std::vector<int> &xs, T gain) {
    const int y = n_ + 2 + m_++;
    bonus_ += gain;
    if_X_and_notY_then_cost(y, kSink, gain);
    for (const auto &x : xs) {
      if_X_and_notY_then_cost(x, y, BIG_COST);
    }
  }

  // {X=0,Y=0} or {X=1,Y=1} => gain
  void if_X_equal_Y_then_gain(int x, int y, T gain) {
    if_all_of_X_then_gain({x, y}, gain);
    if_none_of_X_then_gain({x, y}, gain);
  }

  // {X=0,Y=1} or {X=1,Y=0} => cost
  void if_X_not_equal_Y_then_cost(int x, int y, T cost) {
    if_X_and_notY_then_cost(x, y, cost);
    if_X_and_notY_then_cost(y, x, cost);
  }
};

auto solve() {
  i64 N, W;
  cin >> N >> W;
  vector<i64> A(N);
  cin >> A;
  ProjectSelection<i64> ps(N);
  REP(i, N) { ps.constraint(i, W - A[i]); }
  REP(i, N) {
    int k;
    cin >> k;
    REP(j, k) {
      int c;
      cin >> c;
      --c;
      ps.constraint(c, ~i, BIG_COST);
    }
  }
  return ps.max_gain();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
