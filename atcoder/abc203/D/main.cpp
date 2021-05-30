#include <bits/stdc++.h>

#include <atcoder/fenwicktree>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>
#include <ext/pb_ds/tree_policy.hpp>

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

template <class T>
struct CumSum2d {
  template <class X>
  using Vec = std::vector<X>;

  int n, m;
  Vec<Vec<T>> cum;

  template <class U>
  explicit CumSum2d(const Vec<Vec<U>> &data)
      : n(data.size()), m(data[0].size()), cum(n + 1, Vec<T>(m + 1, T(0))) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        cum[i + 1][j + 1] =
            cum[i][j + 1] + cum[i + 1][j] - cum[i][j] + data[i][j];
      }
    }
  }

  // (r0, c0) := top-left point
  // (r1, c1) := bottom-right point
  T sum(int r0, int c0, int r1, int c1) const {
    assert(r0 <= r1 and c0 <= c1);
    return cum[r1][c1] - cum[r1][c0] - cum[r0][c1] + cum[r0][c0];
  }
};

using namespace std;

const i64 BIG = 1e9;

auto solve() {
  int n, K;
  cin >> n >> K;
  auto a = vector(n, vector(n, 0LL));
  REP(i, n) REP(j, n) { cin >> a[i][j]; }
  const i64 half = (i64(K) * K + 1) / 2;

  auto check = [&](i64 med) -> bool {
    auto b = vector(n, vector(n, 0));
    REP(i, n) REP(j, n) {
      if (a[i][j] <= med) b[i][j] = 1;
    }
    CumSum2d<i64> c(b);
    for (int i0 = 0; i0 + K <= n; ++i0) {
      for (int j0 = 0; j0 + K <= n; ++j0) {
        i64 count = c.sum(i0, j0, i0 + K, j0 + K);
        if (count >= half) return true;
      }
    }
    return false;
  };

  i64 fv = -1, tv = BIG;
  while (tv - fv > 1) {
    i64 mid = (fv + tv) / 2;
    if (check(mid)) {
      tv = mid;
    } else {
      fv = mid;
    }
  }
  return tv;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
