#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

struct Void {};  // No print.

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, Void>) {
    return std::cout;  // Nothing.
  } else if constexpr (std::is_same_v<T, bool>) {
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

// Golden Section Search over an integer interval.
// Finds x in [low, high) s.t. func(x) is minimal.
// Unlike ternary search, `f(x)` is not required to be convex.
template<class F, class T = std::invoke_result_t<F, Int>>
Int find_max_golden_section_search(Int low, Int high, F f) {
  static_assert(std::is_invocable_v<F, Int>);
  std::greater<T> compare;

  --low;             // Make it an open interval: (low, high).
  Int l = 1, r = 1;  // Left and right offsets from `low`.
  while (l + r < high - low) {
    l += r;
    std::swap(l, r);
  }
  auto lval = f(low + l), rval = f(low + r);
  while (l < r) {
    r -= l;
    std::swap(l, r);
    if (compare(lval, rval)) {
      rval = std::move(lval);
      lval = f(low + l);
    } else {
      low += r;
      if (low + r < high) {
        lval = std::move(rval);
        rval = f(low + r);
      } else {
        r -= l;
        std::swap(l, r);
        lval = f(low + l);
      }
    }
  }
  return low + 1;
}

// Binary search:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class T, class F>
auto bisect(T true_x, T false_x, F pred) -> T {
  static_assert(std::is_invocable_r_v<bool, F, T>);
  assert(std::max(true_x, false_x) <= std::numeric_limits<T>::max() / 2);
  assert(true_x >= -1 and false_x >= -1);  // need floor_div to allow negative.

  while (std::abs(true_x - false_x) > 1) {
    T mid = (true_x + false_x) >> 1;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

Int ask_inversion(int l, int r) {
  if (r - l < 2) return 0;
  ++l;
  cout << "? " << l << " " << r << endl;
  Int ret;
  cin >> ret;
  return ret;
}

auto solve() {
  int n;
  cin >> n;
  int l = bisect<int>(0, n, [&](int x) {
    return ask_inversion(0, x) == 0;
  });
  DUMP(l);
  int r = bisect<int>(n, 0, [&](int x) {
    return ask_inversion(x, n) == 0;
  });
  DUMP(r);
  Int mid = find_max_golden_section_search(l, r, [&](int x) {
    return ask_inversion(l, x);
  });
  DUMP(mid);
  cout << "! " << (l + 1) << " " << (mid + 1) << " " << (r + 1) << endl;
}

int main() {
  const int T = in;
  REP(t, T) {
    solve();
  }
}
