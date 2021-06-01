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
using Int = u64;  //__uint128_t;

auto solve() {
  string S;
  cin >> S;
  const int n = S.size();
  auto dp = vector(n + 1, vector(n + 1, char(0)));
  for (int i = n - 1; i >= 0; --i) {
    {
      // odd length
      int maxlen = 0;
      bool alive = true;
      for (int len = 1; alive; ++len) {
        alive = false;
        if (i - len >= 0 and i + len < n) {
          if (S[i - len] == S[i + len]) {
            alive = true;
            maxlen = len;
          }
        }
      }
      for (int j = 1; j <= maxlen; ++j) {
        const int l = i - j;
        const int r = i + j + 1;
        dp[l][r] = 1;
        for (int p = r + 1; p <= n; ++p) {
          if (dp[r][p]) dp[l][p] = 1;
        }
      }
    }
    {
      // even length
      int maxlen = 0;
      bool alive = true;
      for (int len = 1; alive; ++len) {
        alive = false;
        if (i - len >= 0 and i + len - 1 < n) {
          if (S[i - len] == S[i + len - 1]) {
            alive = true;
            maxlen = len;
          }
        }
      }
      for (int j = 1; j <= maxlen; ++j) {
        int l = i - j;
        int r = i + j;
        dp[l][r] = 1;
        for (int p = r + 1; p <= n; ++p) {
          if (dp[r][p]) dp[l][p] = 1;
        }
      }
    }
  }
  Int ans = 0;
  for (int l = 0; l < n; ++l) {
    for (int r = l + 2; r <= n; ++r) {
      if (dp[l][r]) ++ans;
    }
  }
  return (u64)ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
