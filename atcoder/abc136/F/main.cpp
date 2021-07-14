#include <bits/stdc++.h>

#include <atcoder/fenwicktree>
#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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

template <typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(v) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  int index(T x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  const T &value(int i) const { return values[i]; }
};

Mint solve() {
  int n;
  cin >> n;
  vector<pair<int, int>> ps(n);
  vector<int> ys(n);
  REP(i, n) {
    int x, y;
    cin >> x >> y;
    ps[i] = {x, y};
    ys[i] = y;
  }
  Compressed<int> yc(ys);
  atcoder::fenwick_tree<int> ybit(n);
  sort(ALL(ps));
  Mint ans = 0;
  vector<Mint> pow2(n + 1);
  pow2[0] = 1;
  REP(i, n) pow2[i + 1] = pow2[i] * 2;
  const Mint all_count = pow2[n];
  REP(i, n) {
    const auto [x, y] = ps[i];
    const int l = i;
    const int r = n - l - 1;
    const int b = yc.index(y);
    const int u = n - b - 1;
    const int bl = ybit.sum(0, b);
    const int ul = i - bl;
    const int ur = u - ul;
    const int br = b - bl;
    // DUMP(l, r, u, b);
    // DUMP(ul, ur, bl, br);
    Mint ex = 0;
    ex += pow2[u] - 1;
    ex += pow2[b] - 1;
    ex += pow2[l] - 1;
    ex += pow2[r] - 1;
    ex -= pow2[ul] - 1;
    ex -= pow2[ur] - 1;
    ex -= pow2[bl] - 1;
    ex -= pow2[br] - 1;
    ans += (all_count - 1) - ex;
    ybit.add(b, 1);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
