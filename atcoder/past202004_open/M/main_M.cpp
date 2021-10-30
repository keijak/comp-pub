#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

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
  return (int)a.size();
}
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

template <class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template <class T>
inline std::ostream &print(const T &x) {
  return print_one(x, '\n');
}
template <typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template <typename Container>
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n", std::ostream &os = std::cout) {
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

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct CastInput {
  template <typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template <typename T>
    operator T() const {
      T x(n);
      for (auto &e : x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "backward.hpp"
#include "debug_dump.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr \
  if (false) std::cerr
#endif

using namespace std;

// Computes node transition and aggregates values along transition.
template <typename Monoid>
struct AggFunctionalGraph {
 private:
  using T = typename Monoid::T;
  static const int kMaxBits = 60;

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

  // Sets value `x` at node `pos`.
  void set_value(int pos, T value) { acc_value[0][pos] = value; }

  // Sets `next` as the next position of node `pos`.
  void set_next(int pos, int next) { next_pos[0][pos] = next; }

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
    // steps >= 2^kMaxBits is not supported.
    assert(steps < (1LL << kMaxBits));
    build();

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

  long long min_steps(int start, std::function<bool(const T &, int)> pred) {
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

template <class T>
T saturating_add(T x, T y) {
  static_assert(std::is_integral<T>::value);
  T res;
  if (not __builtin_add_overflow(x, y, &res)) {
    return res;
  } else if (x < 0) {
    return std::numeric_limits<T>::lowest();
  } else {
    return std::numeric_limits<T>::max();
  }
}

struct SumOp {
  using T = Int;
  static T op(const T &x, const T &y) { return saturating_add(x, y); }
  static constexpr T id() { return 0; }
};

template <class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}

auto solve() {
  Int D = in, L = in, n = in;
  vector<Int> C = in(D);
  map<Int, set<Int>> dish_day;
  REP(i, D * 3) {
    Int dish = C[i % D];
    dish_day[dish].insert(i);
  }

  AggFunctionalGraph<SumOp> fg(D);
  {
    map<Int, int> last_index;
    for (int i = D * 2 - 1; i >= 0; --i) {
      if (i < D) {
        auto it = last_index.find(C[i]);
        check(it != last_index.end());
        int li = it->second;
        fg.set_next(i, li % D);
        Int dist = li - i;
        Int val = ceil_div<Int>(dist, L);
        fg.set_value(i, val);
      }
      last_index[C[i % D]] = i;
    }
  }

  REP(i, n) {
    Int K = in, F = in, T = in;
    --F;
    auto it = dish_day[K].lower_bound(F);
    if (it == dish_day[K].end()) {
      print(0);
      continue;
    }
    Int first_good_day = *it;
    Int first_dist = first_good_day - F;
    Int first_cnt = ceil_div<Int>(first_dist, L);
    if (first_cnt >= T) {
      print(0);
      continue;
    }
    T -= first_cnt;
    int start = (int)first_good_day % D;
    Int ans =
        fg.min_steps(start, [&](Int visits, int _) { return visits >= T; });
    print(ans);
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
