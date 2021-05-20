#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <vector>

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

using Int = i64;
using u32 = unsigned;

int mssb_pos(u32 x) {
  static const int CLZ_WIDTH = __builtin_clz(1);
  assert(x != 0);
  return CLZ_WIDTH - __builtin_clz(x);
}
template <typename U>
inline U bit_floor(U x) {
  return U(1) << mssb_pos(x);
}

const Int BIG = Int(2e18) + 5;

auto solve() -> i64 {
  int n, k;
  cin >> n >> k;
  vector<array<i64, 2>> pos(n);
  REP(i, n) {
    i64 x, y;
    cin >> x >> y;
    pos[i] = {x, y};
  }
  const unsigned kFull = (1 << n) - 1;
  auto dp = vector(1 << n, vector(k + 1, BIG));
  dp[0][0] = 0;
  for (unsigned bits = 1; bits <= kFull; ++bits) {
    vector<int> ix;
    REP(i, n) {
      if (bits >> i & 1) ix.push_back(i);
    }
    i64 dmax = 0;
    REP(i, ssize(ix)) {
      auto [x1, y1] = pos[ix[i]];
      REP(j, i) {
        auto [x2, y2] = pos[ix[j]];
        Int dx = x1 - x2;
        Int dy = y1 - y2;
        chmax(dmax, dx * dx + dy * dy);
      }
    }
    dp[bits][1] = dmax;
  }
  // DUMP(dp[1]);

  for (unsigned bits = 1; bits <= kFull; ++bits) {
    unsigned mssb = bit_floor(bits);
    for (unsigned sub = bits - mssb; sub; sub = (sub - 1) & bits) {
      unsigned other = bits & ~sub;
      for (int j = 2; j <= k; ++j) {
        for (int i = 1; i < j; ++i) {
          chmin(dp[bits][j], max(dp[sub][i], dp[other][j - i]));
        }
      }
    }
  }
  return (i64)dp[kFull][k];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
