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

bool solve() {
  i64 n, k;
  cin >> n >> k;
  vector<array<i64, 3>> ans(n);
  i64 csum = 0;
  REP(i, n) {
    i64 c = k + 3 * n - 1 - i;
    ans[i][2] = c;
    csum += c;
  }
  set<i64> avail;
  i64 xsum = 0;
  for (i64 x = k; x < k + 2 * n; ++x) {
    avail.insert(x);
    xsum += x;
  }
  if (xsum > csum) return false;

  int i = 0;
  for (i64 b = k + 2 * n - 1; i < n and b >= k; --b) {
    if (not avail.count(b)) continue;
    assert(b < ans[i][2]);
    avail.erase(b);
    i64 am = ans[i][2] - b;
    auto it = avail.upper_bound(am);
    if (it == avail.begin()) return false;
    --it;
    ans[i][0] = *it;
    ans[i][1] = b;
    avail.erase(it);
    ++i;
  }
  if (i < n) return false;
  REP(i, n) {
    auto [a, b, c] = ans[i];
    cout << a << " " << b << " " << c << "\n";
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  if (not solve()) {
    cout << -1 << endl;
  }
}
