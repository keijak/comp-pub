// #define NDEBUG
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

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

// Rational number with lazy reduction.
struct LazyRational {
  using i64 = long long;
  using i128 = __int128_t;

  mutable i64 nume_;
  mutable i64 deno_;
  mutable bool reduced_;

  static constexpr i64 kReduceThreshold = i64(1) << 62;

  LazyRational() : nume_(0), deno_(1), reduced_(true) {}
  LazyRational(i64 n) : nume_(n), deno_(1), reduced_(true) {}

  template<typename T>
  LazyRational(T n, T d) : reduced_(false) {
    assert(d != 0);  // zero division
    if (n == 0) {
      d = 1;
      reduced_ = true;
    } else {
      const int sign = ((n < 0) xor (d < 0)) ? -1 : 1;
      if (n < 0) n = -n;
      if (d < 0) d = -n;
      if (std::max<T>(n, d) > kReduceThreshold) {
        reduce_(n, d);
      }
      n *= sign;
    }
    nume_ = (i64) n;
    deno_ = (i64) d;
  }

  i64 numerator() const {
    if (not reduced_) reduce_me_();
    return nume_;
  }

  i64 denominator() const {
    if (not reduced_) reduce_me_();
    return deno_;
  }

  // Cast to a floating point number.
  template<typename Float>
  explicit operator Float() const {
    static_assert(std::is_floating_point<Float>::value);
    return (Float) nume_ / (Float) deno_;
  }

  const LazyRational &operator+() const { return *this; }

  LazyRational operator-() const {
    LazyRational ret;
    ret.nume_ = -this->nume_;
    ret.deno_ = this->deno_;
    ret.reduced_ = this->reduced_;
    return ret;
  }

  friend bool operator==(const LazyRational &x, const LazyRational &y) {
    return i128(x.nume_) * y.deno_ == i128(x.deno_) * y.nume_;
  }
  friend bool operator!=(const LazyRational &x, const LazyRational &y) {
    return not(x == y);
  }
  friend bool operator<(const LazyRational &x, const LazyRational &y) {
    return i128(x.nume_) * y.deno_ < i128(x.deno_) * y.nume_;
  }
  friend bool operator>(const LazyRational &x, const LazyRational &y) { return y < x; }
  friend bool operator<=(const LazyRational &x, const LazyRational &y) {
    return not(x > y);
  }
  friend bool operator>=(const LazyRational &x, const LazyRational &y) {
    return not(x < y);
  }

  LazyRational &operator+=(const LazyRational &y) {
    i128 zn = i128(this->nume_) * y.deno_ + i128(y.nume_) * this->deno_;
    i128 zd = i128(this->deno_) * y.deno_;
    this->reduced_ = light_reduce_(zn, zd);
    this->nume_ = zn;
    this->deno_ = zd;
    return *this;
  }
  friend LazyRational operator+(LazyRational x, const LazyRational &y) {
    x += y;
    return x;
  }

  friend LazyRational operator-(const LazyRational &x, const LazyRational &y) {
    return x + (-y);
  }
  LazyRational &operator-=(const LazyRational &y) {
    *this += -y;
    return *this;
  }

  LazyRational &operator*=(const LazyRational &y) {
    if (this->nume_ == 0) return *this;
    if (y.nume_ == 0) {
      this->nume_ = 0;
      this->deno_ = 1;
      this->reduced_ = true;
      return *this;
    }
    i128 zn = i128(this->nume_) * y.nume_;
    i128 zd = i128(this->deno_) * y.deno_;
    this->reduced_ = light_reduce_(zn, zd);
    this->nume_ = zn;
    this->deno_ = zd;
    return *this;
  }
  friend LazyRational operator*(LazyRational x, const LazyRational &y) {
    x *= y;
    return x;
  }

  LazyRational inv() const {
    LazyRational ret;
    int sign = this->nume_ >= 0 ? 1 : -1;
    ret.nume_ = this->deno_ * sign;
    ret.deno_ = this->nume_ * sign;
    ret.reduced_ = this->reduced_;
    return ret;
  }
  friend LazyRational operator/(const LazyRational &x, const LazyRational &y) {
    return x * y.inv();
  }
  LazyRational &operator/=(const LazyRational &y) {
    *this *= y.inv();
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &os, const LazyRational &x) {
    return os << x.numerator() << "/" << x.denominator();
  }

 private:
  template<typename T>
  static void reduce_(T &n, T &d) {
    if (n == 0) {
      d = 1;
      return;
    }
    i128 a = (n < 0 ? -n : n), b = d;
    while (b) {
      a %= b;
      std::swap(a, b);
    }
    n /= a;
    d /= a;
  }

  // Returns whether the args are reduced.
  template<typename T>
  static bool light_reduce_(T &n, T &d) {
    if (std::max<T>((n < 0 ? -n : n), d) <= kReduceThreshold) {
      return false;
    }
    reduce_(n, d);
    if constexpr (std::is_same_v<T, i128>) {
      if (std::max<T>((n < 0 ? -n : n), d) > std::numeric_limits<i64>::max()) {
        throw std::overflow_error("cannot fit in 64 bits");
      }
    }
    return true;
  }

  void reduce_me_() const {
    reduce_<i64>(nume_, deno_);
    reduced_ = true;
  }
};
using Rat = LazyRational;

template<class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}

using namespace std;

auto solve() -> Int {
  int p = in, q = in, a = in, n = in;
  if (p == 0 and q == 0 and a == 0 and n == 0) {
    exit_();
  }

  auto f = Rec([&](auto &rec, int i, const Rat &r, Int d0, Int prod) -> Int {
    if (r.nume_ == 0) return 1;
    if (i == 0) return 0;
    Int res = 0;
    for (Int d = max<Int>(d0, ceil_div(r.deno_, r.nume_)); prod * d <= a; ++d) {
      res += rec(i - 1, r - Rat(1LL, d), d, prod * d);
    }
    return res;
  });
  return f(n, Rat(p, q), 1, 1);
}

int main() {
  init_();
  for (int t = 0;; ++t) {
    test_case(t, 0);
    out(solve());
  }
}
