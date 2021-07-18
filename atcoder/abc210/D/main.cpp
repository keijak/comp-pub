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
  for (auto &x : a) is >> x;
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

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<int sign = 1>
struct Infinity {
  template<typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      return T(sign) * (std::numeric_limits<T>::max() / T(2));
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
constexpr Infinity<> INF;

auto solve() {
  INPUT(i64, H, W, C);
  auto A = vector(H, vector(W, 0LL));
  REP(i, H) {
    cin >> A[i];
  }
  DUMP(A);
  i64 ans = INF;
  REP(flip, 2) {
    auto dp = vector(H, vector(W, (i64) INF));
    dp[0][0] = A[0][0];
    REP(i, H) {
      REP(j, W) {
        if (i == 0 and j == 0) continue;
        i64 b = INF;
        if (i > 0) {
          chmin(b, dp[i - 1][j]);
        }
        if (j > 0) {
          chmin(b, dp[i][j - 1]);
        }
        assert(b != INF);
        i64 d = C * (i + j);
        chmin(ans, A[i][j] + d + b);
        dp[i][j] = min(A[i][j] - d, b);
      }
    }

    REP(i, H) {
      for (int j = 0; j < W; ++j) {
        int r = W - 1 - j;
        if (r <= j) break;
        swap(A[i][j], A[i][r]);
      }
    }
  }
  return ans;
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
