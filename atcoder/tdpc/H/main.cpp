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

struct Item {
  i64 weight;
  i64 value;
  int color;
};
bool operator<(const Item &x, const Item &y) {
  return tie(x.color, x.weight, x.value) < tie(y.color, y.weight, y.value);
}
ostream &operator<<(ostream &os, const Item &x) {
  return os << "{w=" << x.weight << ", v=" << x.value << ", c=" << x.color
            << "}";
}

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

auto solve() {
  int n, W, C;
  cin >> n >> W >> C;
  vector<Item> items(n);
  for (auto &x : items) {
    cin >> x.weight >> x.value >> x.color;
    x.color--;
  }
  sort(ALL(items));

  auto dp = vector(2, vector(W + 1, vector(C + 1, vector(2, (i64)-INF))));
  dp[0][0][0][0] = 0;

  REP(i, n) {
    const Item &x = items[i];
    bool new_color = (i == 0) or (x.color != items[i - 1].color);
    REP(j, W + 1) REP(c, C + 1) REP(t, 2) { dp[1][j][c][t] = -INF; }
    REP(j, W + 1) REP(c, C + 1) REP(tail_color, 2) {
      bool tc = new_color ? false : tail_color;
      chmax(dp[1][j][c][tc], dp[0][j][c][tail_color]);
    }
    for (int j = 0; j + x.weight <= W; ++j) {
      REP(c, C + 1) REP(tail_color, 2) {
        if (dp[0][j][c][tail_color] < 0) continue;
        int nc = c + int(new_color or not tail_color);
        if (nc > C) continue;
        chmax(dp[1][j + x.weight][nc][1], dp[0][j][c][tail_color] + x.value);
      }
    }
    swap(dp[0], dp[1]);
  }

  i64 ans = 0;
  REP(j, W + 1) REP(c, C + 1) REP(tc, 2) { chmax(ans, dp[0][j][c][tc]); }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
