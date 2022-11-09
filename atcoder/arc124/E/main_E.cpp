// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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

template<typename T = Mint>
T binom(Int n, Int k) {
  if (k < 0 or k > n) return 0;
  if (k > n - k) k = n - k;
  T nume = 1, deno = 1;
  for (int i = 0; i < k; ++i) {
    nume *= T(n - i);
    deno *= T(i + 1);
  }
  return nume / deno;
}

template<typename T>
struct Array4d {
  std::vector<T> data_;
  size_t b0_ = 0, b1_ = 0, b2_ = 0;

  Array4d(size_t d0, size_t d1, size_t d2, size_t d3)
      : data_(d0 * d1 * d2 * d3, T{}),
        b0_(d1 * d2 * d3),
        b1_(d2 * d3),
        b2_(d3) {}

  inline T &get(size_t i0, size_t i1, size_t i2, size_t i3) {
    return data_[i0 * b0_ + i1 * b1_ + i2 * b2_ + i3];
  }
  // Fills one row with the specified value.
  inline void fill(size_t i0, T val) {
    std::fill(data_.begin() + i0 * b0_, data_.begin() + (i0 + 1) * b0_, val);
  }
};

auto solve() {
  int n = in;
  vector<int> A = in(n);
  Array4d<Mint> dp(n + 1, 2, 2, 2);
  REP(j0, 2) REP(ji, 2) REP(zero, 2) {
        if (zero and ji) continue;
        int from = A[0] + (not zero and ji);
        int val = ji + (1 - j0) + (1 - zero);
        dp.get(0, j0, ji, zero) += binom(from, val);
      }
  for (int i = 1; i < n; ++i) {
    REP(j0, 2) REP(jp, 2) REP(ji, 2) REP(zero, 2) REP(zerop, 2) {
              if (zero and ji) continue;
              if (i == n - 1 and ji != j0) continue;
              int from = A[i] + (not zero and ji);
              int val = ji + (1 - jp) + (1 - zero);
              dp.get(i, j0, ji, max(zero, zerop)) += dp.get(i - 1, j0, jp, zerop) * binom(from, val);
            }
  }
  Mint ans = 0;
  REP(j0, 2) {
    ans += dp.get(n - 1, j0, j0, 1);
  }
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
