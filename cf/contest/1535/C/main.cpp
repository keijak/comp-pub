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

using namespace std;

auto solve() {
  string s;
  cin >> s;
  const int n = s.size();
  // 0: unknown
  // 1: 1 at odd, 0 at even
  // 2: 0 at odd, 1 at even
  vector<int> ptype(n, 0), rknown(n, -1), rdiff(n, -1);
  REP(i, n) {
    if (s[i] == '0') {
      ptype[i] = (i % 2 == 0) ? 1 : 2;
    } else if (s[i] == '1') {
      ptype[i] = (i % 2 == 0) ? 2 : 1;
    }
  }
  {
    int last_known = -1;
    int last1 = -1, last2 = -1;
    REP(i, n) {
      if (s[i] != '?') {
        last_known = i;
      }
      rknown[i] = last_known;
      if (ptype[i] == 1) {
        rdiff[i] = last2;
        last1 = i;
      }
      if (ptype[i] == 2) {
        rdiff[i] = last1;
        last2 = i;
      }
    }
  }
  i64 ans = 0;
  REP(i, n) {
    i64 l = i;
    if (rknown[i] < 0) {
      l = 0;
    } else {
      int r = rknown[i];
      if (rdiff[r] < 0) {
        l = 0;
      } else {
        l = rdiff[r] + 1;
      }
    }
    ans += i - l + 1;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) { cout << solve() << "\n"; }
}
