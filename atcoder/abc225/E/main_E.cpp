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

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
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
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
#define cerr if(false)std::cerr
#endif

using namespace std;
using D = long double;      // Coordinate value

// Rational number
template<typename T, bool allow_infinity = false>
struct Rational {
 public:
  // #include <boost/multiprecision/cpp_int.hpp>
  // using multip = boost::multiprecision;
  // using BigInt = boost::multiprecision::checked_int128_t;
  using BigInt = __int128_t;

  T nume, deno;

  Rational() : nume(0), deno(1) {}

  Rational(T n) : nume(n), deno(1) {}

  Rational(BigInt n, BigInt d) {
    if constexpr (allow_infinity) {
      assert(d != 0 or n != 0);  // 0/0 is undefined
    } else {
      assert(d != 0);
    }
    if (allow_infinity and d == 0) {
      n = (n < 0) ? -1 : 1;  // infinity
    } else if (n == 0) {
      d = 1;
    } else {
      const int sign = ((n < 0) xor (d < 0)) ? -1 : 1;
      if (n < 0) n = -n;
      if (d < 0) d = -d;
      const auto g = gcd_(n, d);
      if (g > 1) {
        n /= g;
        d /= g;
      }
      n *= sign;
    }
    nume = static_cast<T>(n);
    deno = static_cast<T>(d);
  }

  Rational(const Rational &x) = default;
  Rational(Rational &&x) = default;
  Rational &operator=(const Rational &x) = default;
  Rational &operator=(Rational &&x) = default;

  // Cast to a floating point number.
  template<typename Float>
  explicit operator Float() const {
    static_assert(std::is_floating_point<Float>::value);
    return (Float) nume / (Float) deno;
  }

  Rational operator+() const { return *this; }
  Rational operator-() const {
    Rational ret;
    ret.nume = -nume;
    ret.deno = deno;
    return ret;
  }

  friend bool operator==(const Rational &x, const Rational &y) {
    return (x.nume == y.nume) and (x.deno == y.deno);
  }
  friend bool operator!=(const Rational &x, const Rational &y) {
    return not(x == y);
  }
  friend bool operator<(const Rational &x, const Rational &y) {
    if constexpr (allow_infinity) {
      if (x.deno == 0 and y.deno == 0) return x.nume < y.nume;
    }
    return static_cast<BigInt>(x.nume) * y.deno <
        static_cast<BigInt>(y.nume) * x.deno;
  }
  friend bool operator>(const Rational &x, const Rational &y) { return y < x; }
  friend bool operator<=(const Rational &x, const Rational &y) {
    return not(x > y);
  }
  friend bool operator>=(const Rational &x, const Rational &y) {
    return not(x < y);
  }

  friend Rational operator+(const Rational &x, const Rational &y) {
    if constexpr (allow_infinity) {
      if (x.deno == 0 and y.deno == 0) {
        assert(x.nume == y.nume);  // (infinity - infinity) is undefined
        return x;
      }
      if (x.deno == 0) return x;
      if (y.deno == 0) return y;
    }
    auto g = gcd_(x.deno, y.deno);
    BigInt xd = x.deno / g;
    BigInt yd = y.deno / g;
    BigInt zn = x.nume * yd + y.nume * xd;
    BigInt zd = xd * y.deno;
    return Rational(std::move(zn), std::move(zd));
  }
  Rational &operator+=(const Rational &x) { return (*this = *this + x); }

  friend Rational operator-(const Rational &x, const Rational &y) {
    return x + (-y);
  }
  Rational &operator-=(const Rational &x) { return (*this = *this - x); }

  friend Rational operator*(const Rational &x, const Rational &y) {
    if (x.nume == 0 or y.nume == 0) return Rational(0);
    auto g1 = gcd_(abs_(x.nume), y.deno);
    auto g2 = gcd_(abs_(y.nume), x.deno);
    return Rational(BigInt(x.nume / g1) * BigInt(y.nume / g2),
                    BigInt(x.deno / g2) * BigInt(y.deno / g1));
  }
  Rational &operator*=(const Rational &x) { return (*this = *this * x); }

  Rational inv() const { return Rational(deno, nume); }

  friend Rational operator/(const Rational &x, const Rational &y) {
    return x * y.inv();
  }
  Rational &operator/=(const Rational &x) { return (*this = *this / x); }

  friend std::ostream &operator<<(std::ostream &os, const Rational &x) {
    return os << x.nume << "/" << x.deno;
  }

 private:
  template<typename U>
  static inline U abs_(const U &x) {
    // return std::abs(x);
    // return boost::multiprecision::abs(x);
    return (x < 0) ? -x : x;
  }
  template<typename U>
  static U gcd_(const U &a, const U &b) {
    // return std::gcd(a, b);
    return (b == 0) ? a : gcd_(b, a % b);
  }
};
using Rat = Rational<long long, true>;

auto solve() {
  int n = in;
  vector<tuple<Rat, Rat, int>> ps(n);
  REP(i, n) {
    int x = in, y = in;
    ps[i] = tuple{Rat(y, x - 1), Rat(y - 1, x), i};
  }
  sort(ALL(ps));
  Rat ma = {-1, 0};
  int cnt = 0;
  REP(i, n) {
    Rat l, r;
    int j;
    tie(r, l, j) = ps[i];
    if (l >= ma) {
      ++cnt;
      ma = r;
    }
  }
  return cnt;
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
