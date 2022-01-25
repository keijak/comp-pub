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

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

template<class T>
struct CumSum2d {
  template<class U>
  using Vec = std::vector<U>;

  int n, m;
  Vec<Vec<T>> cum;

  template<class U>
  explicit CumSum2d(const Vec<Vec<U>> &data)
      : n(data.size()), m(data.at(0).size()), cum(n + 1, Vec<T>(m + 1, T(0))) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        cum[i + 1][j + 1] =
            cum[i][j + 1] + cum[i + 1][j] - cum[i][j] + data[i][j];
      }
    }
  }

  // [x_lo, x_hi) x [y_lo, y_hi)
  T sum(int x_lo, int x_hi, int y_lo, int y_hi) const {
    assert(x_lo <= x_hi and y_lo <= y_hi);
    return cum[x_hi][y_hi] - cum[x_hi][y_lo] - cum[x_lo][y_hi] +
        cum[x_lo][y_lo];
  }
};

auto solve() {
  int H = in, W = in;
  auto A = vector(H, vector(W, 0LL));
  Int asum = 0;
  REP(i, H) {
    REP(j, W) {
      cin >> A[i][j];
      asum += A[i][j];
    }
  }
  CumSum2d<Int> sa(A);
  auto rowcheck = [&](int rx, int ry, Int d) -> optional<vector<int>> {
    Int s = 0;
    vector<int> ret = {ry};
    ret.reserve(50);
    for (int i = ry; i < W; ++i) {
      s += sa.sum(0, rx, i, i + 1);
      if (s > d) return nullopt;
      if (s == d) {
        ret.push_back(i + 1);
        s = 0;
      }
    }
    if (s != 0) return nullopt;
    return ret;
  };
  auto colcheck = [&](int rx, int ry, Int d) -> optional<vector<int>> {
    Int s = 0;
    vector<int> ret = {rx};
    ret.reserve(50);
    for (int i = rx; i < H; ++i) {
      s += sa.sum(i, i + 1, 0, ry);
      if (s > d) return nullopt;
      if (s == d) {
        ret.push_back(i + 1);
        s = 0;
      }
    }
    if (s != 0) return nullopt;
    return ret;
  };

  int cnt = 0;
  for (int rx = 1; rx <= H; ++rx) {
    for (int ry = 1; ry <= W; ++ry) {
      if (rx == H and ry == W) break;
      auto rs = sa.sum(0, rx, 0, ry);
      if (asum % rs != 0) continue;
      auto vl = rowcheck(rx, ry, rs);
      if (not vl.has_value()) continue;
      auto hl = colcheck(rx, ry, rs);
      if (not hl.has_value()) continue;
      auto ok = [&]() {
        for (int i = 1; i < ssize(*hl); ++i) {
          for (int j = 1; j < ssize(*vl); ++j) {
            Int s = sa.sum((*hl)[i - 1], (*hl)[i], (*vl)[j - 1], (*vl)[j]);
            if (s != rs) return false;
          }
        }
        return true;
      }();
      if (ok) {
        DUMP(rx, ry, *vl, *hl);
        ++cnt;
      }
    }
  }
  return cnt;
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
