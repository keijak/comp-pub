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

#include <boost/multiprecision/cpp_int.hpp>
using Int = boost::multiprecision::cpp_int;

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// Factorizes a number into {prime, count} pairs. O(sqrt(n)).
map<i64, int> factorize(i64 n) {
  map<i64, int> res;
  if (n == 1) return res;
  for (i64 k = 2; k * k <= n; ++k) {
    if (n % k != 0) continue;
    int count = 0;
    do {
      n /= k;
      ++count;
    } while (n % k == 0);
    res[k] = count;
  }
  if (n > 1) {
    res[n] = 1;
  }
  return res;
}

void multiply(map<i64, int> &f1, map<i64, int> &f2) {
  for (auto [p, k] : f2) {
    f1[p] += k;
  }
}

optional<Int> get_sqrt(const map<i64, int> &f1) {
  Int res = 1;
  for (auto [p, k] : f1) {
    if (k % 2 == 1) return nullopt;
    REP(i, k / 2) res *= p;
  }
  return res;
}

bool solve() {
  i64 a, b, c, d;
  cin >> a >> b >> c >> d;
  optional<Int> sqrt_res;
  if (a == 0 or b == 0 or c == 0 or d == 0) {
    sqrt_res = 0;
  } else {
    auto fa = factorize(a);
    auto fb = factorize(b);
    auto fc = factorize(c);
    auto fd = factorize(d);
    multiply(fa, fb);
    multiply(fa, fc);
    multiply(fa, fd);
    sqrt_res = get_sqrt(fa);
  }

  Int na = a, nb = b, nc = c, nd = d;
  Int left = na + nb - nc - nd;
  if (left < 0) {
    if (nc * nd >= na * nb) return false;
  } else if (left > 0) {
    if (nc * nd <= na * nb) return false;
  } else if (left == 0) {
    return nc * nd == na * nb;
  }
  if (not sqrt_res.has_value()) return false;
  DUMP(sqrt_res.value());
  DUMP(left);
  left = left * left;
  Int right = 4 * (nc * nd + na * nb - 2 * sqrt_res.value());
  return left == right;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << (solve() ? "Yes" : "No") << "\n";
}
