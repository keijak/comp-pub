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

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
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
#endif

using namespace std;

template<typename Monoid, int kNumBits = 45>
struct AggFunctionalGraph {
 public:
  using T = typename Monoid::T;

  // number of nodes.
  int size;

  // acc_value[d][i] := starting from i, what's the value accumulated after 2^d
  // steps.
  std::vector<std::vector<T>> acc_value;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  bool build_done_;

  // Builds transition tables.
  void build_() {
    if (build_done_) return;
    for (int d = 0; d + 1 < kNumBits; ++d) {
      for (int i = 0; i < size; ++i) {
        int p = next_pos[d][i];
        next_pos[d + 1][i] = next_pos[d][p];
        acc_value[d + 1][i] = Monoid::op(acc_value[d][i], acc_value[d][p]);
      }
    }
    build_done_ = true;
  }

 public:
  explicit AggFunctionalGraph(int n)
      : size(n),
        acc_value(kNumBits, std::vector<T>(n, Monoid::id())),
        next_pos(kNumBits, std::vector<int>(n)),
        build_done_(false) {
    for (int d = 0; d < kNumBits; ++d) {
      for (int i = 0; i < n; ++i) {
        next_pos[d][i] = i;  // self-loop by default
      }
    }
  }

  // Sets `next` as the next position of node `pos`.
  void set_next(int pos, int next) { next_pos[0][pos] = next; }

  // Sets value `x` at node `pos`.
  void set_value(int pos, T value) { acc_value[0][pos] = value; }

  // Starting from `start`, `steps` times goes forward and accumulates values.
  std::pair<T, int> go(int start, long long steps) {
    // steps >= 2^kNumBits is not supported.
    assert(steps < (1LL << kNumBits));
    build_();

    T agg = Monoid::id();
    int cur_node = start;
    for (int d = kNumBits - 1; d >= 0; --d) {
      if ((steps >> d) & 1) {
        agg = Monoid::op(agg, acc_value[d][cur_node]);
        cur_node = next_pos[d][cur_node];
      }
    }
    return {agg, cur_node};
  }

  // Returns the minimum steps to reach pred() == true.
  template<class F>
  tuple<int, T> min_steps(int start, F pred) {
    static_assert(std::is_invocable_r_v<bool, F, T, int>);
    build_();
    T cur_agg = Monoid::id();
    int cur_node = start;
    for (int d = kNumBits - 1; d >= 0;) {
      T next_agg = Monoid::op(cur_agg, acc_value[d][cur_node]);
      int next_node = next_pos[d][cur_node];
      if (pred(next_agg, cur_node)) {
        // Retry with a smaller jump.
        --d;
      } else {
        // Continue to jump by the same d.
        std::swap(cur_agg, next_agg);
        std::swap(cur_node, next_node);
      }
    }
    return {cur_node, cur_agg};
  }
};

template<class T>
T saturating_add(T x, T y) {
  static_assert(std::is_integral<T>::value);
  static constexpr T kMin = std::numeric_limits<T>::lowest();
  static constexpr T kMax = std::numeric_limits<T>::max();
  static_assert(kMax != 0);
  if (T res; not __builtin_add_overflow(x, y, &res)) {
    return res;
  } else if constexpr (not std::is_signed<T>::value) {
    return kMax;
  } else {
    return (x < 0) ? kMin : kMax;
  }
}

#include <boost/multiprecision/cpp_int.hpp>
using BigInt = boost::multiprecision::checked_int256_t;
namespace multip = boost::multiprecision;

struct SumOp {
  using T = long long;
  static T op(const T &x, const T &y) {
    return saturating_add(x, y);
    //return x + y;
  }
  static T id() { return 0; }
};

auto solve() {
  int n = in;
  Int K = in;
  vector<int> a = in(n);
  AggFunctionalGraph<SumOp> g(n);
  vector<int> pre(200005, -1);
  for (int i = n * 2 - 1; i >= 0; --i) {
    int j = pre[a[i % n]];
    if (i < n) {
      assert(j != -1);
      int from = i;
      int to = (j + 1) % n;
      int jump = (j + 1) - i;
      g.set_next(from, to);
      g.set_value(from, jump);
    }
    pre[a[i % n]] = i;
  }
  const Int nk = n * K;
  auto[from, v] = g.min_steps(0, [&](const auto &pos, int j) {
    return pos >= nk;
  });
  vector<int> ans;
  for (auto j = v; j < nk;) {
    int pos = int(j % n);
    auto jump = g.acc_value[0][pos];
    if (j + jump > nk) {
      ans.push_back(a[pos]);
      ++j;
    } else {
      j += jump;
    }
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print_seq(ans);
  }
}
