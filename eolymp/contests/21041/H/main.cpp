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
using u64 = unsigned long long;

#include <boost/multiprecision/cpp_int.hpp>
using Int = boost::multiprecision::cpp_int;

Int solve(int n, int d) {
  if (n % 2 == 1) return 0;
  if (d == 0) return 0;
  if (2 * d > n) return 0;
  if (2 * d == n) return 1;

  auto dp = vector(n + 1, vector(d + 1, Int(-1)));

  auto f = [&](auto &f, int m, int k) -> Int {
    if (m == 0) return 1;
    if (k == 0) return 0;
    assert(m % 2 == 0);
    if (dp[m][k] >= 0) return dp[m][k];
    DUMP(m, k);
    Int res = 0;
    for (int i = 1; i < m; i += 2) {
      int l = i - 1;
      int r = m - 1 - i;
      auto lv = f(f, l, k - 1);
      if (lv > 0) {
        res += lv * f(f, r, k);
      }
    }
    return dp[m][k] = res;
  };
  Int ans = f(f, n, d) - f(f, n, d - 1);
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  string s;
  int n, d;
  while (getline(cin, s)) {
    stringstream ss(s);
    if (not(ss >> n)) continue;
    if (not(ss >> d)) continue;
    cout << solve(n, d) << "\n";
  }
}
