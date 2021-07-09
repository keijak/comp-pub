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
  read_from_cin(__VA_ARGS__)

#ifdef ENABLE_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
inline int popcount(unsigned x) { return __builtin_popcount(x); }

auto solve(int K) {
  const int kOffset = 300;
  const int M = K + kOffset + 10;
  const unsigned kFull = (1 << 16) - 1;
  auto dp = vector(1 << 16, vector(M, 0LL));
  dp[0][kOffset] = 1;
  REP(bits, kFull) {
    int m = popcount(bits);
    int x = m + 1;
    const auto &dp1 = dp[bits];
    REP(i, 16) {
      if (bits >> i & 1) continue;
      unsigned next_bits = bits | (1 << i);
      int r = i / 4;
      int c = i % 4;
      int delta = 0;
      vector<unsigned> ps;
      if (r > 0) {
        ps.push_back((r - 1) * 4 + c);
      }
      if (r < 3) {
        ps.push_back((r + 1) * 4 + c);
      }
      if (c > 0) {
        ps.push_back(r * 4 + c - 1);
      }
      if (c < 3) {
        ps.push_back(r * 4 + c + 1);
      }
      for (auto p : ps) {
        if (bits >> p & 1) {
          delta += x;
        } else {
          delta -= x;
        }
      }
      auto &dp2 = dp[next_bits];
      for (int score = max(0, -delta); score + delta < M; ++score) {
        int next_score = score + delta;
        dp2[next_score] += dp1[score];
      }
    }
  }
  return dp[kFull][kOffset + K];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << "{0";
  for (int i = 1; i <= 216; ++i) {
    cout << "," << solve(i);
    cout.flush();
  }
  cout << "}";
}
