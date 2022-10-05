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

template<typename T>
struct Array5d {
  std::vector<T> data_;
  size_t b0_ = 0, b1_ = 0, b2_ = 0, b3_ = 0;

  Array5d(size_t d0, size_t d1, size_t d2, size_t d3, size_t d4)
      : data_(d0 * d1 * d2 * d3 * d4, T{}),
        b0_(d1 * d2 * d3 * d4),
        b1_(d2 * d3 * d4),
        b2_(d3 * d4),
        b3_(d4) {}
  inline T &get(size_t i0, size_t i1, size_t i2, size_t i3, size_t i4) {
    return data_[i0 * b0_ + i1 * b1_ + i2 * b2_ + i3 * b3_ + i4];
  }
  // Fills one row with the specified value.
  inline void fill(size_t i0, T val) {
    std::fill(data_.begin() + i0 * b0_, data_.begin() + (i0 + 1) * b0_, val);
  }
};

auto solve() {
  int n = in;
  vector<int> A = in(n);
  Array5d<Mint> dp(n + 1, 2, 2, 2, 2);
  dp.get(0, 0, 0, 0, 0) = A[0];
  dp.get(0, 0, 0, 1, 1) = A[0];
  dp.get(0, 1, 1, 0, 0) = A[n - 1];
  dp.get(0, 1, 1, 1, 1) = A[n - 1];

  auto debug = [&](int i) {
    REP(prev, 2) REP(prev0, 2) REP(stop, 2) REP(someone_stop, 2) {
            auto val = dp.get(i, prev, prev0, stop, someone_stop);
            DUMP(i, prev, prev0, stop, someone_stop, val);
          }
  };
  REP(i, n - 2) {
    REP(j0, 2) REP(ke, 2) {
        if (A[i + 1] > 1) {
          auto &cur = dp.get(i + 1, 0, j0, 0, ke);
          cur += dp.get(i, 0, j0, 0, ke) * A[i + 1];
          cur += dp.get(i, 0, j0, 1, ke) * A[i + 1];
          cur += dp.get(i, 1, j0, 0, ke) * A[i + 1];
          cur += dp.get(i, 1, j0, 1, ke) * A[i + 1];
        }
        if (A[i + 1] > 0) {
          auto &cur = dp.get(i + 1, 0, j0, 1, 1);
          cur += dp.get(i, 0, j0, 0, ke) * A[i + 1];
          cur += dp.get(i, 0, j0, 1, ke) * A[i + 1];
          cur += dp.get(i, 1, j0, 0, ke) * A[i + 1];
          cur += dp.get(i, 1, j0, 1, ke) * A[i + 1];
        }
        if (A[i] > 0) {
          auto &cur = dp.get(i + 1, 1, j0, 0, ke);
          cur += dp.get(i, 0, j0, 0, ke) * (A[i] - 1);
          cur += dp.get(i, 1, j0, 0, ke) * A[i];
        }
        if (A[i] > 0) {
          auto &cur = dp.get(i + 1, 1, j0, 1, 1);
          cur += dp.get(i, 0, j0, 0, ke) * (A[i] - 1);
          cur += dp.get(i, 1, j0, 0, ke) * A[i];
        }
      }
  }
  {
    const int i = n - 2;
    REP(j0, 2) REP(ke, 2) {
        if (ke and A[i + 1] - j0 > 1) {
          auto &cur = dp.get(i + 1, 0, j0, 0, ke);
          cur += dp.get(i, 0, j0, 0, ke) * (A[i + 1] - j0);
          cur += dp.get(i, 0, j0, 1, ke) * (A[i + 1] - j0);
          cur += dp.get(i, 1, j0, 0, ke) * (A[i + 1] - j0);
          cur += dp.get(i, 1, j0, 1, ke) * (A[i + 1] - j0);
        }
        if (not j0 and A[i + 1] - j0 > 0) {
          auto &cur = dp.get(i + 1, 0, j0, 1, 1);
          cur += dp.get(i, 0, j0, 0, ke) * (A[i + 1] - j0);
          cur += dp.get(i, 0, j0, 1, ke) * (A[i + 1] - j0);
          cur += dp.get(i, 1, j0, 0, ke) * (A[i + 1] - j0);
          cur += dp.get(i, 1, j0, 1, ke) * (A[i + 1] - j0);
        }
        if (ke and A[i] > 0) {
          auto &cur = dp.get(i + 1, 1, j0, 0, ke);
          cur += dp.get(i, 0, j0, 0, ke) * (A[i] - 1);
          cur += dp.get(i, 1, j0, 0, ke) * A[i];
        }
        if (not j0 and A[i] > 0) {
          auto &cur = dp.get(i + 1, 1, j0, 1, 1);
          cur += dp.get(i, 0, j0, 0, ke) * (A[i] - 1);
          cur += dp.get(i, 1, j0, 0, ke) * A[i];
        }
      }
  }
  Mint ans = 0;
  REP(j, 2) REP(j0, 2) REP(k, 2) {
        ans += dp.get(n - 1, j, j0, k, 1);
      }
  out(ans);

//  REP(t, 3) {
//    test_case(t, 3);
//    debug(t);
//  }
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
