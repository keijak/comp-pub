#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u32 = unsigned;

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
const int L = 30;

u32 solve() {
  u32 n;
  cin >> n;
  vector<u32> a(n), b(n);
  cin >> a >> b;
  u32 ans = 0;
  REP(i, L) {
    const u32 mask = (1LL << (i + 1)) - 1;
    vector<u32> as(n);
    REP(j, n) {
      u32 ax = a[j] & mask;
      as[j] = ax;
    }
    sort(ALL(as));
    i64 count = 0;
    REP(j, n) {
      i64 bx = b[j] & mask;
      {
        i64 y = (1LL << (i + 1)) - bx;
        int p = lower_bound(ALL(as), y) - as.begin();
        i64 z = (1LL << i) - bx;
        int q = lower_bound(ALL(as), z) - as.begin();
        count += p - q;
      }
      {
        i64 z = (1 << (i + 1)) + (1 << i) - bx;
        int p = lower_bound(ALL(as), z) - as.begin();
        count += n - p;
      }
    }
    if (count % 2 == 1) {
      ans |= (1 << i);
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
