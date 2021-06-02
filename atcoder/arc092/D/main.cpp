#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u32 = unsigned;

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
const int L = 28;

i64 solve() {
  u32 n;
  cin >> n;
  vector<u32> a(n), b(n);
  cin >> a >> b;
  auto bit_counts = vector<u32>(L, 0);
  REP(i, L) {
    u32 count = 0;
    REP(j, n) {
      if (b[j] >> i & 1) count ^= 1;
    }
    bit_counts[i] = count;
  }
  u32 ans = 0;
  u32 carry = 0;
  REP(i, L) {
    u32 a1 = 0;
    REP(j, n) {
      if (a[j] >> i & 1) a1 ^= 1;
    }
    u32 a0 = (n - a1) & 1;
    u32 b1 = bit_counts[i];
    u32 b0 = (n - b1) & 1;
    ones *z
  }
}
//
return -42;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
