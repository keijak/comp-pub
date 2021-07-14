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
  int n;
  cin >> n;
  vector<i64> a(n);
  cin >> a;
  vector<tuple<i64, bool, i64>> du, dd;
  du.reserve(n * 2);
  dd.reserve(n * 2);

  i64 delta_sum = 0;
  REP(i, 1, n) {
    i64 x = a[i] - a[i - 1];
    if (x >= 0) {
      du.push_back({a[i - 1], false, a[i]});
      du.push_back({a[i], true, a[i - 1]});
    } else {
      dd.push_back({a[i], false, a[i - 1]});
      dd.push_back({a[i - 1], true, a[i]});
    }
    delta_sum += abs(x);
  }
  i64 ans = delta_sum;
  REP(i, 1, n) {
    i64 x = a[i] - a[i - 1];
    i64 d = delta_sum - abs(x) + abs(a[i] - a[0]);
    chmin(ans, d);
  }
  for (int i = n - 2; i >= 0; --i) {
    i64 x = a[i] - a[i + 1];
    i64 d = delta_sum - abs(x) + abs(a[i] - a[n - 1]);
    chmin(ans, d);
  }
  DUMP(ans);

  auto f = [&](vector<tuple<i64, bool, i64>> &ds) {
    sort(ALL(ds));
    multiset<i64> s;
    for (const auto &[x, ending, y] : ds) {
      if (not ending) {
        s.insert(x);
      } else {
        auto it = s.find(y);
        s.erase(it);
        if (!s.empty()) {
          i64 z = *s.begin();
          i64 d = delta_sum - 2 * (x - max(z, y));
          chmin(ans, d);
        }
      }
    }
  };
  f(du);
  f(dd);
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
