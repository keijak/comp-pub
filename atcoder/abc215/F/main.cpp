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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    return (std::cin >> x), x;
  }
} in;

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

// Binary search.
// Returns the boundary argument which satisfies pred(x).
//
// Usage:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class F>
i64 bisect(i64 true_x, i64 false_x, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, i64>, "F must be: i64 -> bool");
  assert(std::max<i64>(true_x, false_x) <= std::numeric_limits<i64>::max() / 2);
  // To allow negative values, use floor_div() in the loop.
  assert(true_x >= -1 and false_x >= -1);
  using u64 = unsigned long long;

  while (std::abs(true_x - false_x) > 1) {
    i64 mid = ((u64) true_x + (u64) false_x) / 2;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

auto solve() {
  int n = in;
  vector<pair<i64, i64>> xy(n);
  i64 xmin = INF, xmax = -INF, ymin = INF, ymax = -INF;
  REP(i, n) {
    i64 x = in, y = in;
    xy[i] = {x, y};
    chmin(xmin, x);
    chmax(xmax, x);
    chmin(ymin, y);
    chmax(ymax, y);
  }
  sort(ALL(xy));

  i64 dmax = max(xmax - xmin, ymax - ymin);
  DUMP(xy, dmax);
  auto ans = bisect(dmax, -1LL, [&](i64 d) -> bool {
    i64 yma = -INF, ymi = INF;
    int head = 0;
    REP(i, n) {
      i64 x, y;
      tie(x, y) = xy[i];
      while (head < n and xy[head].first < x - d) {
        auto[xh, yh] = xy[head];
        chmax(yma, yh);
        chmin(ymi, yh);
        head++;
      }
      if (ymi != INF) {
        if (abs(yma - y) > d or abs(y - ymi) > d) return false;
      }
    }
    return true;
  });
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