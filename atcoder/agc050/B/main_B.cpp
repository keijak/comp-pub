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

// N-dimensional array.
template<typename T, int dim>
struct NdArray {
  std::vector<T> data_;
  std::array<int, dim> shape_;
  std::array<int, dim> base_;

  NdArray(const std::array<int, dim> &shape, T init = T{})
      : data_(std::accumulate(shape.begin(), shape.end(), size_t(1),
                              [](size_t x, size_t y) { return x * y; }),
              std::move(init)),
        shape_(shape) {
    size_t b = 1;
    for (int j = dim - 1; j >= 0; --j) {
      base_[j] = b;
      b *= shape_[j];
    }
  }

  T &operator[](const std::array<int, dim> &index) {
    size_t pos = 0;
    for (int j = dim - 1; j >= 0; --j) {
      pos += index[j] * base_[j];
    }
    return data_[pos];
  }

  // Fills one row with the specified value.
  inline void fill(int i0, T val) {
    std::fill(data_.begin() + i0 * base_[0],
              data_.begin() + (i0 + 1) * base_[0], val);
  }
};

auto solve() {
  int n = in;
  vector<Int> a = in(n);
  NdArray<Int, 6> dp({n + 10, 2, 2, n + 10, 2, 2}, -kBigVal<Int>);
  REP(l, n + 1) {
    REP(r, l, n + 1) {
      dp[{l, 0, 0, r, 0, 0}] = 0;
    }
  }
  for (int width = n - 1; width >= 0; --width) {
    for (int l = 0; l + width <= n; ++l) {
      const int r = l + width;
      REP(x0, 2) {
        REP(x1, 2) REP(y0, 2) REP(y1, 2) {
              if (r + 3 <= n) {
                auto val = dp[{l, x0, x1, r + 3, y0, y1}];
                if (val == -kBigVal<Int>) continue;
                chmax(dp[{l, x0, x1, r, 1, 1}], val + a[r] + a[r + 1] + a[r + 2]);
              }
              if (r + 4 < n and y0 and y1) {
                auto val = dp[{l, x0, x1, r + 3, y0, y1}];
                if (val == -kBigVal<Int>) continue;
                chmax(dp[{l, x0, x1, r, 1, 1}], val + a[r] + a[r + 1] - a[r + 3] - a[r + 4]);
              }
              if (r + 3 < n and y0) {
                auto val = dp[{l, x0, x1, r + 3, y0, y1}];
                if (val == -kBigVal<Int>) continue;
                chmax(dp[{l, x0, x1, r, 1, 0}], val + a[r] - a[r + 3]);
              }
              if (r + 1 < n and y0) {
                auto val = dp[{l, x0, x1, r + 1, y0, y1}];
                if (val == -kBigVal<Int>) continue;
                chmax(dp[{l, x0, x1, r, 0, 1}], val);
              }
              if (r + 2 <= n) {
                auto val = dp[{l, x0, x1, r + 2, y0, y1}];
                if (val == -kBigVal<Int>) continue;
                chmax(dp[{l, x0, x1, r, 0, 0}], val);

              }
              if (l - 3 >= 0) {
                auto val = dp[{l - 3, x0, x1, r, y0, y1}];
                if (val == -kBigVal<Int>) continue;
                chmax(dp[{l, 1, 1, r, y0, y1}], val + a[l - 1] + a[l - 2] + a[l - 3]);
              }
              if (l - 5 >= 0 and x0 and x1) {
                auto val = dp[{l - 3, x0, x1, r, y0, y1}];
                if (val == -kBigVal<Int>) continue;
                chmax(dp[{l, 1, 1, r, y0, y1}], val + a[l - 1] + a[l - 2] - a[l - 4] - a[l - 5]);
              }
              if (l - 4 >= 0 and x0) {
                auto val = dp[{l - 3, x0, x1, r, y0, y1}];
                if (val == -kBigVal<Int>) continue;
                chmax(dp[{l, 1, 0, r, y0, y1}], val + a[l - 1] - a[l - 4]);
              }
              if (l - 2 >= 0 and x0) {
                auto val = dp[{l - 1, x0, x1, r, y0, y1}];
                if (val == -kBigVal<Int>) continue;
                chmax(dp[{l, 0, 1, r, y0, y1}], val);
              }
              if (l - 2 >= 0) {
                auto val = dp[{l - 2, x0, x1, r, y0, y1}];
                if (val == -kBigVal<Int>) continue;
                chmax(dp[{l, 0, 0, r, y0, y1}], val);
              }
            }
      }

    }
  }
  Int ans = 0;
  for (int l = 0; l <= n; ++l) {
    for (int r = l; r <= n; ++r) {
      REP(x0, 2) {
        REP(x1, 2) REP(y0, 2) REP(y1, 2) {
              chmax(ans, dp[{l, x0, x1, r, y0, y1}]);
            }
      }
    }
  }
  return ans;
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
