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

#ifdef MY_DEBUG
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
const Infinity<> kBig;

using namespace std;

i64 solve() {
  int n;
  cin >> n;
  const int n2 = n * 2;
  vector<i64> R, G, B;
  REP(i, n2) {
    i64 cute;
    char color;
    cin >> cute >> color;
    if (color == 'R') {
      R.push_back(cute);
    } else if (color == 'G') {
      G.push_back(cute);
    } else {
      assert(color == 'B');
      B.push_back(cute);
    }
  }
  if (ssize(R) % 2 == 0 and ssize(G) % 2 == 0 and ssize(B) % 2 == 0) {
    return 0;
  }
  if (ssize(R) % 2 == 0) {
    swap(R, B);
  } else if (ssize(G) % 2 == 0) {
    swap(G, B);
  }
  sort(ALL(R));
  sort(ALL(G));
  sort(ALL(B));
  DUMP(R, G, B);  // R,G:odd B:even
  i64 ans = abs(R[0] - G[0]);
  // pair R and G.
  REP(i, ssize(R)) {
    auto it = lower_bound(ALL(G), R[i]);
    if (it != G.end()) chmin(ans, abs(R[i] - *it));
    if (it != G.begin()) chmin(ans, abs(R[i] - *prev(it)));
  }
  // pair B-R and B-G
  vector<i64> br, bg;
  REP(i, ssize(B)) {
    {
      i64 d = kBig;
      auto it = lower_bound(ALL(R), B[i]);
      if (it != R.end()) chmin(d, abs(B[i] - *it));
      if (it != R.begin()) chmin(d, abs(B[i] - *prev(it)));
      if (d != kBig) br.emplace_back(d);
    }
    {
      i64 d = kBig;
      auto it = lower_bound(ALL(G), B[i]);
      if (it != G.end()) chmin(d, abs(B[i] - *it));
      if (it != G.begin()) chmin(d, abs(B[i] - *prev(it)));
      if (d != kBig) bg.emplace_back(d);
    }
  }
  if (ssize(br) > 0 and ssize(bg) > 0) {
    chmin(ans, *min_element(ALL(br)) + *min_element(ALL(bg)));
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
