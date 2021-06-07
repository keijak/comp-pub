#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
#include <bits/stdc++.h>

template <typename T = long long,
          typename = std::enable_if_t<std::is_integral<T>::value>>
struct Rational {
 private:
  using BigInt = __int128_t;

 public:
  T nume, deno;

  Rational() : nume(0), deno(1) {}

  explicit Rational(T n) : nume(n), deno(1) {}

  Rational(T n, T d) : nume(n), deno(d) {
    assert(deno != 0);
    if (nume == 0) {
      deno = 1;
    } else {
      int sign = 1;
      if ((nume < 0) xor (deno < 0)) {
        sign = -1;
      }
      nume = abs(nume);
      deno = abs(deno);
      auto g = std::gcd(nume, deno);
      if (g > 1) {
        nume /= g;
        deno /= g;
      }
      nume *= sign;
    }
  }

  operator double() { return (double)(nume) / deno; }
  operator long double() { return (long double)(nume) / deno; }

  friend bool operator<(const Rational &x, const Rational &y) {
    return (BigInt)(x.nume) * y.deno < (BigInt)(y.nume) * x.deno;
  }

  friend bool operator==(const Rational &x, const Rational &y) {
    return std::tie(x.nume, x.deno) == std::tie(y.nume, y.deno);
  }

  friend Rational operator+(const Rational &x, const Rational &y) {
    auto g = std::gcd(x.deno, y.deno);
    auto zn = x.nume * (y.deno / g) + y.nume * (x.deno / g);
    auto zd = (x.deno / g) * y.deno;
    return Rational(zn, zd);
  }

  friend Rational operator*(const Rational &x, const Rational &y) {
    auto g1 = std::gcd(std::abs(x.nume), y.deno);
    auto g2 = std::gcd(std::abs(y.nume), x.deno);
    return Rational((x.nume / g1) * (y.nume / g2),
                    (x.deno / g2) * (y.deno / g1));
  }

  friend std::ostream &operator<<(std::ostream &os, const Rational &x) {
    return os << x.nume << "/" << x.deno;
  }
};

vector<i64> solve() {
  int n;
  cin >> n;
  string s;
  cin >> s;
  vector<i64> dcum(n + 1, 0LL);
  REP(i, n) { dcum[i + 1] = dcum[i] + int(s[i] == 'D'); }

  vector<i64> ans(n, 1);
  map<Rational<>, int> mp;
  REP(i, n) {
    Rational<> r(dcum[i + 1], i64(i + 1));
    int c = ++mp[r];
    chmax(ans[i], c);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) { print_seq(solve()); }
}
