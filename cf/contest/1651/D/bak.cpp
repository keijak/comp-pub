#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<typename T>
struct MergeSegmentTree {
  int n_;
  std::vector<std::vector<T>> data_;  // Rows sorted by T: data_[x][y] = value.

  explicit MergeSegmentTree(std::vector<std::vector<T>> data)
      : n_(data.size()), data_(2 * n_) {
    for (int i = n_, n2 = n_ * 2; i < n2; ++i) {
      data_[i] = std::move(data[i - n_]);
      std::sort(data_[i].begin(), data_[i].end());
    }
    for (int i = n_ - 1; i >= 1; --i)
      std::merge(data_[i << 1 | 0].begin(), data_[i << 1 | 0].end(),
                 data_[i << 1 | 1].begin(), data_[i << 1 | 1].end(),
                 std::back_inserter(data_[i]));
  }

  // Returns the nubmer of points in the range [x_lo, x_hi) x [y_lo, y_hi).
  // O(N(logN)^2)
  int count(int x_lo, int x_hi, T y_lo, T y_hi) const {
    int res = 0;
    for (x_lo += n_, x_hi += n_; x_lo < x_hi; x_lo >>= 1, x_hi >>= 1) {
      if (x_lo & 1) res += count_(x_lo++, y_lo, y_hi);
      if (x_hi & 1) res += count_(--x_hi, y_lo, y_hi);
    }
    return res;
  }

  // Returns all points in the range [x_lo, x_hi) x [y_lo, y_hi).
  // O(N(logN)^2 + |output|)
  std::vector<T> collect(int x_lo, int x_hi, T y_lo, T y_hi) const {
    std::vector<T> res;
    for (x_lo += n_, x_hi += n_; x_lo < x_hi; x_lo >>= 1, x_hi >>= 1) {
      if (x_lo & 1) {
        collect_(x_lo++, y_lo, y_hi, res);
        if (not res.empty()) break;
      }
      if (x_hi & 1) {
        collect_(--x_hi, y_lo, y_hi, res);
        if (not res.empty()) break;
      }
    }
    return res;
  }

 private:
  int count_(int i, T y_lo, T y_hi) const {
    auto lo_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_lo);
    auto hi_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_hi);
    return int(hi_it - lo_it);
  }
  void collect_(int i, T y_lo, T y_hi, std::vector<T> &out) const {
    auto lo_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_lo);
    auto hi_it = std::lower_bound(data_[i].begin(), data_[i].end(), y_hi);
    if (lo_it != hi_it) out.push_back(*lo_it);
    //std::copy(lo_it, hi_it, std::back_inserter(out));
  }
};

// Binary search over integers
template<class T, class F>
auto bisect(T truthy, T falsy, F pred) -> T {
  static_assert(std::is_integral_v<T>);
  static_assert(std::is_invocable_r_v<bool, F, T>);
  while (std::max(truthy, falsy) - std::min(truthy, falsy) > T(1)) {
    auto mid = (truthy & falsy) + (truthy ^ falsy) / T(2);
    auto ok = pred(mid);
    (ok ? truthy : falsy) = std::move(mid);
  }
  return truthy;
}

auto solve() {
  int n = in;
  vector<pair<int, int>> xy(n);
  set<pair<int, int>> xyset;

  static const int dx[] = {1, 0, -1, 0};
  static const int dy[] = {0, 1, 0, -1};

  auto adjacent = [&](int x, int y) -> std::vector<std::pair<int, int>> {
    std::vector<std::pair<int, int>> ret;
    for (size_t d = 0; d < std::size(dx); ++d) {
      int nx = x + dx[d];
      int ny = y + dy[d];
      ret.emplace_back(nx, ny);
    }
    return ret;
  };

  REP(i, n) {
    int x = in, y = in;
    xy[i] = {x, y};
    xyset.emplace(x, y);
  }

  int L = 400005;
  vector<vector<pair<int, int>>> uv(L);
  REP(i, n) {
    auto[x, y] = xy[i];
    for (auto[nx, ny]: adjacent(x, y)) {
      if (xyset.count({nx, ny})) continue;
      int u = nx + ny;
      int v = nx - ny;
      uv[u].push_back({v, u});
    }
  }
  REP(i, L) {
    if (not uv[i].empty()) sort(ALL(uv[i]));
  }
  while (not uv.empty() and uv.back().empty()) uv.pop_back();
  L = ssize(uv);

  // value: (v,u)
  MergeSegmentTree<pair<int, int>> st(uv);

  REP(i, n) {
    const auto[x, y] = xy[i];
    if (fastcheck(x, y)) continue;
    const int qu = x + y;
    const int qv = x - y;

    auto rect = [&](int d) -> array<int, 4> {
      return {max(qu - d, 0), min(qu + d + 1, L), qv - d, qv + d + 1};
    };
    int dist = bisect<int>(3 * L, 0, [&](int d) {
      auto r = rect(d);
      return st.count(r[0], r[1], {r[2], -kBigVal<int>}, {r[3], -kBigVal<int>}) > 0;
    });
    auto r = rect(dist);
    auto res = st.collect(r[0], r[1], {r[2], -kBigVal<int>}, {r[3], -kBigVal<int>});
    assert(res.size() >= 1);
    {
      auto[v1, u1] = res.front();
      int x1 = (u1 + v1) / 2;
      int y1 = (u1 - v1) / 2;
      print(x1, y1);
    }
  }
  return Void();
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
