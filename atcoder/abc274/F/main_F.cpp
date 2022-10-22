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

using namespace std;

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
  LazyRational(i64 n, i64 d, bool reduced)
      : nume_(n), deno_(d), reduced_(reduced) {}

  template<typename T>
  LazyRational(T n, T d) : reduced_(false) {
    assert(d != 0);  // zero division
    if (n == 0) {
      d = 1;
      reduced_ = true;
    } else {
      reduced_ = maybe_reduce_(n, d);
    }
    nume_ = static_cast<i64>(n);
    deno_ = static_cast<i64>(d);
  }

  i64 numerator() const {
    if (not reduced_) {
      reduce_(nume_, deno_);
      reduced_ = true;
    }
    return nume_;
  }

  i64 denominator() const {
    if (not reduced_) {
      reduce_(nume_, deno_);
      reduced_ = true;
    }
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
    return LazyRational(-nume_, deno_, reduced_);
  }

  LazyRational &operator+=(const LazyRational &y) {
    if (y.nume_ == 0) return *this;
    if (this->nume_ == 0) {
      *this = y;
      return *this;
    }
    i128 zn = i128(this->nume_) * y.deno_ + i128(y.nume_) * this->deno_;
    i128 zd = i128(this->deno_) * y.deno_;
    this->reduced_ = maybe_reduce_(zn, zd);
    this->nume_ = static_cast<i64>(zn);
    this->deno_ = static_cast<i64>(zd);
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
    } else {
      i128 zn = i128(this->nume_) * y.nume_;
      i128 zd = i128(this->deno_) * y.deno_;
      this->reduced_ = maybe_reduce_(zn, zd);
      this->nume_ = static_cast<i64>(zn);
      this->deno_ = static_cast<i64>(zd);
    }
    return *this;
  }
  friend LazyRational operator*(LazyRational x, const LazyRational &y) {
    x *= y;
    return x;
  }

  LazyRational inv() const {
    int sign = this->nume_ >= 0 ? 1 : -1;
    return LazyRational(this->deno_ * sign, this->nume_ * sign, this->reduced_);
  }
  friend LazyRational operator/(const LazyRational &x, const LazyRational &y) {
    return x * y.inv();
  }
  LazyRational &operator/=(const LazyRational &y) {
    *this *= y.inv();
    return *this;
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
  friend bool operator>(const LazyRational &x, const LazyRational &y) {
    return y < x;
  }
  friend bool operator<=(const LazyRational &x, const LazyRational &y) {
    return not(x > y);
  }
  friend bool operator>=(const LazyRational &x, const LazyRational &y) {
    return not(x < y);
  }

  friend std::ostream &operator<<(std::ostream &os, const LazyRational &x) {
    return os << x.numerator() << "/" << x.denominator();
  }

 private:
  template<typename T>
  static T abs_(const T &x) {
    if constexpr (std::is_same_v<T, i128>) {
      return x < 0 ? -x : x;
    } else {
      return std::abs(x);
    }
  }

  template<typename T>
  static void reduce_(T &n, T &d) {
    if (n == 0) {
      d = 1;
      return;
    }
    i128 a = abs_(n), b = d;
    while (b) {
      a %= b;
      std::swap(a, b);
    }
    if (a > 1) {
      n /= static_cast<T>(a);
      d /= static_cast<T>(a);
    }
  }

  // Returns whether the args are reduced.
  template<typename T>
  static bool maybe_reduce_(T &n, T &d) {
    if (std::max<T>(abs_(n), d) <= kReduceThreshold) {
      return false;
    }
    reduce_(n, d);
    if constexpr (std::is_same_v<T, i128>) {
      assert(std::max<T>(abs_(n), d) <= std::numeric_limits<i64>::max());
    }
    return true;
  }
};
using Rat = LazyRational;

struct Event {
  Rat t;
  bool in;
  int id;
};

auto solve() {
  int n = in, A = in;
  vector<int> W(n), X(n), V(n);
  REP(i, n) {
    cin >> W[i] >> X[i] >> V[i];
  }

  vector<int> xs, vp, vn;
  auto f = [&](int L) -> Int {
    vector<Event> events;
    Int base = W[L];
    for (int i = 0; i < n; ++i) {
      if (i == L) continue;
      Int x = X[i] - X[L];
      Int v = V[i] - V[L];
      if (v == 0) {
        if (0 <= x and x <= A) {
          base += W[i];
        }
      } else if (v > 0) {
        if (A - x < 0) continue;
        Rat tmin = Rat(max(-x, 0LL), v);
        Rat tmax = Rat(A - x, v);
        events.push_back({tmin, true, i});
        events.push_back({tmax, false, i});
      } else {
        if (x < 0) continue;
        Rat tmin = Rat(max(x - A, 0LL), -v);
        Rat tmax = Rat(x, -v);
        events.push_back({tmin, true, i});
        events.push_back({tmax, false, i});
      }
    }
    sort(ALL(events), [&](const Event &e1, const Event &e2) {
      return pair(e1.t, not e1.in) < pair(e2.t, not e2.in);
    });
    Int ans = base, ws = 0;
    for (const Event &e: events) {
      if (e.in) {
        ws += W[e.id];
      } else {
        ws -= W[e.id];
      }
      chmax(ans, base + ws);
    }
    return ans;
  };

  Int wmax = 0;
  REP(i, n) {
    Int res = f(i);
    chmax(wmax, res);
  }
  out(wmax);
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
