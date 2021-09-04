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
constexpr Infinity<> kBig;

struct P {
  i64 x, y;
};
auto solve() {
  vector<P> points(4);
  REP(i, 4) {
    i64 x = in, y = in;
    points[i] = {x, y};
  }
  vector<int> perm = {0, 1, 2, 3};
  i64 ans = kBig;
  do {
    int tl = perm[0];
    int tr = perm[1];
    int bl = perm[2];
    int br = perm[3];
    array<i64, 2> top_y = {points[tl].y, points[tr].y};
    sort(ALL(top_y));
    array<i64, 2> bottom_y = {points[bl].y, points[br].y};
    sort(ALL(bottom_y));
    array<i64, 2> left_x = {points[tl].x, points[bl].x};
    sort(ALL(left_x));
    array<i64, 2> right_x = {points[tr].x, points[br].x};
    sort(ALL(right_x));

    i64 hmax = top_y[1] - bottom_y[0];
    i64 wmax = right_x[1] - left_x[0];
    if (hmax < 0 or wmax < 0) continue;
    i64 hmin = max(top_y[0] - bottom_y[1], 0LL);
    i64 wmin = max(right_x[0] - left_x[1], 0LL);
    DUMP(perm);
    DUMP(hmin, hmax, wmin, wmax);
    i64 co = top_y[1] - top_y[0];
    co += bottom_y[1] - bottom_y[0];
    co += left_x[1] - left_x[0];
    co += right_x[1] - right_x[0];
    if (hmax < wmin) {
      co += (wmin - hmax) * 2;
    }
    if (wmax < hmin) {
      co += (hmin - wmax) * 2;
    }
    chmin(ans, co);

  } while (next_permutation(ALL(perm)));
  return ans;
}

auto main() -> int {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = in;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}
