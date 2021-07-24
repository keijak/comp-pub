#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
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
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<typename T>
struct Rational {
 public:
  using BigInt = __int128_t;
  // using BigInt = multip::checked_int128_t; // for testing overflow

  T nume, deno;

  Rational() : nume(0), deno(1) {}

  Rational(T n) : nume(n), deno(1) {}

  Rational(BigInt n, BigInt d) {
    assert(d != 0);
    if (n == 0) {
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
      assert(n <= std::numeric_limits<T>::max());
      assert(d <= std::numeric_limits<T>::max());
      n *= sign;
    }
    nume = static_cast<T>(n);
    deno = static_cast<T>(d);
  }

  Rational(const Rational &x) = default;
  Rational(Rational &&x) = default;
  Rational &operator=(const Rational &x) = default;
  Rational &operator=(Rational &&x) = default;

  operator double() const { return (double) nume / (double) deno; }
  operator long double() const { return (long double) nume / (long double) deno; }

  Rational operator+() const { return *this; }
  Rational operator-() const { return Rational(-nume, deno); }

  friend bool operator==(const Rational &x, const Rational &y) {
    return (x.nume == y.nume) and (x.deno == y.deno);
  }
  friend bool operator!=(const Rational &x, const Rational &y) {
    return not(x == y);
  }
  friend bool operator<(const Rational &x, const Rational &y) {
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
    auto g = gcd_(x.deno, y.deno);
    auto zn = (static_cast<BigInt>(x.nume) * (y.deno / g)) +
        (static_cast<BigInt>(y.nume) * (x.deno / g));
    auto zd = static_cast<BigInt>(x.deno / g) * y.deno;
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
    return (x < 0) ? -x : x;
  }
  template<typename U>
  static U gcd_(const U &a, const U &b) {
    return (b == 0) ? a : gcd_(b, a % b);
  }
};
using Rat = Rational<long long>;
// using Rat = Rational<__int128_t>;
// using Rat = Rational<multip::checked_int128_t>; // for testing overflow

// Reads a rational number from the input parsing decimal representation.
// (e.g. "0.2029" => 2029/10000 )
std::istream &operator>>(std::istream &is, Rat &x) {
  long long nume = 0, deno = 1;
  char ch;
  while (is.get(ch)) {
    if (not std::isspace(ch)) break;
  }
  int sgn = 1;
  if (ch == '-') {
    sgn = -1;
    is.get(ch);
  }
  bool in_frac = false;
  while (true) {
    if (not std::isdigit(ch)) {
      is.unget();
      break;
    }
    nume = (nume * 10) + int(ch - '0');
    if (in_frac) deno *= 10;
    if (not(is.get(ch))) break;
    if (ch == '.') {
      in_frac = true;
      if (not(is.get(ch))) break;
    }
  }
  x = Rat(nume * sgn, deno);
  return is;
}

void solve() {
  Rat r;
  cin >> r;
  i64 x = r.deno;
  i64 y = r.nume;
  DUMP(r, x, y);
  i64 z = max(x, y) + (x - 1) + (y - 1);
  if ((x + y) % 2 == 0) {
    print('A', z);
  } else if (x % 2 == 1) {
    print('B', z - 1);
  } else {
    print('C', z - 1);
  }
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    solve();
  }
}
