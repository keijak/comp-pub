// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
//using Mint = atcoder::modint998244353;
using Mint = atcoder::modint1000000007;
//using Mint = atcoder::modint;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
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

// N-dimensional array.
template<typename T, int dim>
struct NdArray {
  std::vector<T> data_;
  std::array<int, dim> shape_;
  std::array<int, dim> base_;

  NdArray(const std::array<int, dim> &shape, T init = T{})
      : data_(std::accumulate(shape.begin(), shape.end(), size_t(1),
                              [](size_t x, size_t y) { return x * y; }),
              std::move(init)),
        shape_(shape) {
    size_t b = 1;
    for (int j = dim - 1; j >= 0; --j) {
      base_[j] = b;
      b *= shape_[j];
    }
  }

  T &operator[](const std::array<int, dim> &index) {
    size_t pos = 0;
    for (int j = dim - 1; j >= 0; --j) {
      pos += index[j] * base_[j];
    }
    return data_[pos];
  }

  // Fills one row with the specified value.
  inline void fill(int i0, T val) {
    std::fill(data_.begin() + i0 * base_[0],
              data_.begin() + (i0 + 1) * base_[0], val);
  }
};

using namespace std;

auto solve() {
  int n = in;
  vector<int> a = in(n);
  auto dp = NdArray<Int, 6>({10, 10, 10, 10, 10, 10});
  for (auto x: a) {
    array<int, 6> ix{};
    REP(i, 6) {
      ix[i] = x % 10;
      x /= 10;
    }
    dp[ix] += 1;
  }

  array<int, 6> from{}, to{};
  REP(d, 6) {
    REP(i, 9) {
      from[d] = i;
      to[d] = i + 1;
      REP(i0, 10) {
        from[0 + (d <= 0)] = to[0 + (d <= 0)] = i0;
        REP(i1, 10) {
          from[1 + (d <= 1)] = to[1 + (d <= 1)] = i1;
          REP(i2, 10) {
            from[2 + (d <= 2)] = to[2 + (d <= 2)] = i2;
            REP(i3, 10) {
              from[3 + (d <= 3)] = to[3 + (d <= 3)] = i3;
              REP(i4, 10) {
                from[4 + (d <= 4)] = to[4 + (d <= 4)] = i4;
                dp[to] += dp[from];
              }
            }
          }
        }
      }
    }
  }

  Int ans = 0;
  for (auto x: a) {
    array<int, 6> ix{};
    bool self = true;
    REP(i, 6) {
      int d = x % 10;
      if (d >= 5) self = false;
      ix[i] = 9 - d;
      x /= 10;
    }
    ans += dp[ix];
    if (self) ans -= 1;
  }
  out(ans / 2);
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
