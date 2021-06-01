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
template <int sign = 1>
struct Infinity {
  template <typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned<T>::value,
                  "must be positive in an unsigned type");
    return (std::numeric_limits<T>::max() / T(2)) * T(sign);
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template <typename T>
  friend bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template <typename T>
  friend bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
const Infinity<> INF;

i64 solve(const string &S) {
  const int n = S.size();
  auto dp = vector(n + 1, vector(n + 1, (i64)INF));
  for (int l = 0; l <= n; ++l) dp[l][l] = 0;
  for (int width = 2; width <= n; width += 2) {
    for (int l = 0; l + width <= n; ++l) {
      const int r = l + width;
      for (int p = l + 1; p < r; ++p) {
        i64 cost = 0;
        if ((S[l] == '(' and S[p] == ')') or (S[l] == '[' and S[p] == ']') or
            (S[l] == '{' and S[p] == '}')) {
          cost = 0;
        } else if (S[l] == '(' or S[l] == '[' or S[l] == '{' or S[p] == ')' or
                   S[p] == ']' or S[p] == '}') {
          cost = 1;
        } else {
          cost = 2;
        }
        if (dp[l + 1][p] != INF and dp[p + 1][r] != INF) {
          chmin(dp[l][r], dp[l + 1][p] + dp[p + 1][r] + cost);
        }
      }
    }
  }
  return dp[0][n];
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  string s;
  while (cin >> s) {
    cout << (i64)solve(s) << "\n";
  }
}
