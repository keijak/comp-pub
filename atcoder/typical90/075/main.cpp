#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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

void print() { std::cout << "\n"; }
template <class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template <typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template <typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__);

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

// Most Significant Set Bit (Highest One Bit) = std::bit_floor(x)
int mssb_pos(unsigned x) {
  static const int CLZ_WIDTH = __builtin_clz(1);
  assert(x != 0);
  return CLZ_WIDTH - __builtin_clz(x);
}
int mssb_pos(u64 x) {
  static const int CLZLL_WIDTH = __builtin_clzll(1LL);
  assert(x != 0);
  return CLZLL_WIDTH - __builtin_clzll(x);
}
template <typename U>
inline U bit_floor(U x) {
  if (x == 0) return 0;
  return U(1) << mssb_pos(x);
}
template <typename U>
U bit_ceil(U x) {
  auto b = bit_floor(x);
  return (b == x) ? b : (b << 1);
}
int bit_width(u64 x) {
  if (x == 0) return 0;
  return std::numeric_limits<u64>::digits - __builtin_clzll(x);
}

// Factorizes a number into {prime, count} pairs. O(sqrt(n)).
unsigned factorize(i64 n) {
  assert(n > 0);
  unsigned res = 0;
  for (i64 k = 2; k * k <= n; ++k) {
    if (n % k != 0) continue;
    do {
      n /= k;
      ++res;
    } while (n % k == 0);
  }
  if (n > 1) {
    ++res;
  }
  return res;
}

auto solve() {
  INPUT(u64, n);
  return mssb_pos(bit_ceil(factorize(n)));
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
