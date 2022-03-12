#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

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

using namespace std;

using Int = unsigned long long;

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

int chromatic_number(const vector<vector<Int>> &G, Int thres) {
  using Mint = atcoder::modint1000000007;
  int n = (int)G.size();
  vector<int> neighbor(n, 0);
  for (int i = 0; i < n; ++i) {
    int S = (1 << i);
    for (int j = 0; j < n; ++j)
      if (G[i][j] > thres) S |= (1 << j);
    neighbor[i] = S;
  }

  // I[S] := #. of inndepndet subset of S
  vector<int> I(1 << n);
  I[0] = 1;
  for (int S = 1; S < (1 << n); ++S) {
    int v = __builtin_ctz(S);
    I[S] = I[S & ~(1 << v)] + I[S & ~neighbor[v]];
  }
  int low = 0, high = n;
  while (high - low > 1) {
    int mid = (low + high) >> 1;

    // g[S] := #. of "k independent sets" which cover S just
    // f[S] := #. of "k independent sets" which consist of subseta of S
    // then
    //   f[S] = sum_{T in S} g(T)
    //   g[S] = sum_{T in S} (-1)^(|S|-|T|)f[T]
    Mint g = 0;
    for (int S = 0; S < (1 << n); ++S) {
      int sign = ((n - __builtin_popcount(S)) & 1) ? -1 : 1;
      g += sign * Mint(I[S]).pow(mid);
    }
    if (g.val() != 0)
      high = mid;
    else
      low = mid;
  }
  return high;
}

auto solve() {
  int n, k;
  cin >> n >> k;
  vector<array<i64, 2>> pos(n);
  REP(i, n) {
    i64 x, y;
    cin >> x >> y;
    pos[i] = {x, y};
  }
  auto g = vector(n, vector(n, Int(0)));
  vector<Int> ds;
  Int dmax = 0;
  REP(i, n) {
    auto [x1, y1] = pos[i];
    REP(j, i) {
      auto [x2, y2] = pos[j];
      Int d1 = x1 - x2;
      Int d2 = y1 - y2;
      g[i][j] = g[j][i] = d1 * d1 + d2 * d2;
      ds.push_back(g[i][j]);
      chmax(dmax, g[i][j]);
    }
  }
  ds.push_back(0);
  Compressed<Int> cds(move(ds));

  Int fv = 0, tv = cds.size();
  while (tv - fv > 1) {
    Int mid = (fv + tv) / 2;
    int c = chromatic_number(g, cds.value(mid));
    if (c > k) {
      fv = mid;
    } else {
      tv = mid;
    }
  }
  return i64(cds.value(tv));
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
