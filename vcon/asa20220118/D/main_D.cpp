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

#include <boost/multiprecision/cpp_int.hpp>
using BigInt = boost::multiprecision::checked_int256_t;
namespace multip = boost::multiprecision;

template<typename Monoid, int kMaxBits = 35>
struct AggFunctionalGraph {
 private:
  using T = typename Monoid::T;

  // number of nodes.
  int size;

  // acc_value[d][i] := starting from i, what's the value accumulated after 2^d
  // steps.
  std::vector<std::vector<T>> acc_value;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  bool build_done_;

 public:
  explicit AggFunctionalGraph(int n)
      : size(n),
        acc_value(kMaxBits, std::vector<T>(n, Monoid::id())),
        next_pos(kMaxBits, std::vector<int>(n)),
        build_done_(false) {
    for (int d = 0; d < kMaxBits; ++d) {
      for (int i = 0; i < n; ++i) {
        next_pos[d][i] = i;  // self-loop by default
      }
    }
  }

  // Sets `next` as the next position of node `pos`.
  void set_next(int pos, int next) { next_pos[0][pos] = next; }

  // Sets value `x` at node `pos`.
  void set_value(int pos, T value) { acc_value[0][pos] = value; }

  // Builds transition tables.
  void build() {
    if (build_done_) return;
    for (int d = 0; d + 1 < kMaxBits; ++d) {
      for (int i = 0; i < size; ++i) {
        int p = next_pos[d][i];
        next_pos[d + 1][i] = next_pos[d][p];
        acc_value[d + 1][i] = Monoid::op(acc_value[d][i], acc_value[d][p]);
      }
    }
    build_done_ = true;
  }

  // Starting from `start`, `steps` times goes forward and accumulates values.
  std::pair<T, int> go(int start, long long steps) {
    build();
    // steps >= 2^kMaxBits is not supported.
    assert(steps < (1LL << kMaxBits));

    T res = Monoid::id();
    int i = start;
    for (int d = kMaxBits - 1; d >= 0; --d) {
      if ((steps >> d) & 1) {
        res = Monoid::op(res, acc_value[d][i]);
        i = next_pos[d][i];
      }
    }
    return {res, i};
  }

  long long min_steps(int start,
                      std::function<bool(const T &, int)> pred) {
    build();
    long long max_false = 0;
    T val = Monoid::id();
    int i = start;
    for (int d = kMaxBits - 1; d >= 0; --d) {
      T tmp = Monoid::op(val, acc_value[d][i]);
      auto j = next_pos[d][i];
      if (pred(tmp, j)) continue;
      max_false += 1LL << d;
      val = std::move(tmp);
      i = j;
    }
    return max_false + 1;
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

struct SumOp {
  using T = Int;
  static T op(const T &x, const T &y) {
    return saturating_add(x, y);
  }
  static constexpr T id() { return 0; }
};

auto solve() {
  Int N = in, X = in, M = in;
  AggFunctionalGraph<SumOp> fg(M);
  REP(i, M) {
    int nx = (Int(i) * i) % M;
    fg.set_next(i, nx);
    fg.set_value(i, i);
  }
  auto[val, pos] = fg.go(X, N);
  return val;
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
