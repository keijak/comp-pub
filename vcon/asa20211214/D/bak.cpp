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

inline int bit_width(unsigned x) {
  if (x == 0) return 0;
  return std::numeric_limits<unsigned>::digits - __builtin_clz(x);
}
inline int bit_width(Uint x) {
  if (x == 0) return 0;
  return std::numeric_limits<Uint>::digits - __builtin_clzll(x);
}
template<typename T, typename U = std::make_unsigned_t<T>>
inline U bit_floor(T x) {
  if (x <= 2) return x;
  return U(1) << (bit_width(U(x)) - 1);
}
template<typename T, typename U = std::make_unsigned_t<T>>
inline U bit_ceil(T x) {
  if (x <= 2) return x;
  return bit_floor(U(x - 1)) << 1;
}
inline int msb_log(Uint x) {
  assert(x != 0);
  return std::numeric_limits<Uint>::digits - __builtin_clzll(x) - 1;
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

template<class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}

auto solve() {
  int n = in, m = in;
  vector<Int> a = in(n);

  Int t = bisect<Int>(2e9, 0, [&](Int z) {
    Int rem = m;
    REP(i, n) {
      Int q = ceil_div<Int>(a[i], z);
      Uint c = bit_ceil<Uint>(q);
      Int y = msb_log(c);
      //DUMP(a[i], z, q, c, y);
      rem -= y;
      if (rem < 0) return false;
    }
    return true;
  });
  DUMP(t);

  Int ans = 0;
  Int rem = m;
  REP(i, n) {
    if (a[i] <= t) {
      ans += a[i];
      DUMP(i, a[i]);
      continue;
    }
    Int q = ceil_div<Int>(a[i], t);
    Uint c = bit_ceil<Uint>(q);
    Int y = msb_log(c);
    DUMP(i, a[i], t, q, c, y, a[i] / c);
    rem -= y;
    assert(rem >= 0);
    ans += a[i] / c;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
