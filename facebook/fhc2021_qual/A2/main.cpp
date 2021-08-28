#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x: a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
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
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

template<int sign = 1>
struct Infinity {
  template<typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      return T(sign) * (std::numeric_limits<T>::max() / T(4));
    }
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template<typename T>
  friend constexpr bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template<typename T>
  friend constexpr bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
constexpr Infinity<> kBig;

using namespace std;

auto solve() {
  const string s = in;
  int n = ssize(s);
  const int K = in;
  auto d = vector(26, vector(26, (i64) kBig));
  REP(i, 26) d[i][i] = 0;
  REP(i, K) {
    char a = in, b = in;
    a -= 'A';
    b -= 'A';
    d[a][b] = 1;
  }
  REP(k, 26) REP(i, 26) REP(j, 26) {
        chmin(d[i][j], d[i][k] + d[k][j]);
      }
  i64 ans = kBig;
  REP(target, 26) {
    i64 cnt = 0;
    REP(j, n) {
      int ch = s[j] - 'A';
      i64 x = d[ch][target];
      if (x == kBig) {
        cnt = kBig;
        break;
      }
      cnt += x;
    }
    if (chmin(ans, cnt)) {
      DUMP(ans, (char) ('A' + target));
    }
  }
  return (ans == kBig) ? -1LL : ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = in;
  REP(test_case, t) {
    auto ans = solve();
    cout << "Case #" << (test_case + 1) << ": ";
    print(ans);
  }
}
