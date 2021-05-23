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
using i128 = boost::multiprecision::int128_t;
using Int = boost::multiprecision::cpp_int;

template <typename T = Int>
T comb(long long n, long k) {
  if (k < 0 or k > n) return 0;
  if (k > n - k) k = n - k;
  T nume = 1, deno = 1;
  for (int i = 0; i < k; ++i) {
    nume *= T(n - i);
    deno *= T(i + 1);
  }
  return nume / deno;
}

void solve() {
  u64 a, b, k;
  cin >> a >> b >> k;
  Int K = k;
  const u64 n = a + b;
  vector<char> s(n);
  REP(i, n) {
    if (K == 0) break;
    if (a > 0) {
      Int x = comb(a - 1 + b, b);
      if (x >= K) {
        --a;
        s[i] = 'a';
        continue;
      }
      K -= x;
    }
    --b;
    s[i] = 'b';
  }
  string ans(ALL(s));
  cout << ans << "\n";
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
