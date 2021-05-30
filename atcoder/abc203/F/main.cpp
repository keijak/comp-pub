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

using namespace std;

void solve() {
  int n, K;
  cin >> n >> K;
  vector<i64> a(n);
  cin >> a;
  sort(ALL(a));
  int mcount = 0;
  {
    int i = n - 1;
    while (i >= 0) {
      ++mcount;
      auto it = upper_bound(ALL(a), a[i] / 2);
      i = (it - a.begin()) - 1;
    }
  }
  DUMP(mcount);

  auto dp3 = vector(n + 1, vector<int>(mcount + 1, (int)INF));
  dp3[n][0] = 0;
  dp3[0][mcount] = 0;
  for (int i = n - 1; i >= 0; --i) {
    const i64 x = a[i];
    const int j = lower_bound(ALL(a), x * 2) - a.begin();
    for (int x = 0; x <= mcount; ++x) {
      chmin(dp3[i][x], dp3[i + 1][x] + 1);
    }
    for (int x = 0; x < mcount; ++x) {
      chmin(dp3[i][x + 1], dp3[j][x]);
    }
  }
  i64 ans = mcount;
  int kmin = 0;
  for (int i = 0; i <= mcount; ++i) {
    if (dp3[0][i] <= K) {
      ans = i;
      kmin = dp3[0][i];
      break;
    }
  }
  cout << ans << " " << kmin << "\n";
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}
