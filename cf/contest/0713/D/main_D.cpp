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
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

template<typename T>
struct Array4d {
  std::unique_ptr<T[]> data_;
  size_t b0_ = 0, b1_ = 0, b2_ = 0;

  Array4d(size_t d0, size_t d1, size_t d2, size_t d3)
      : data_(new T[d0 * d1 * d2 * d3]), b0_(d1 * d2 * d3), b1_(d2 * d3), b2_(d3) {}

  inline T &get(size_t i0, size_t i1, size_t i2, size_t i3) const {
    return data_[i0 * b0_ + i1 * b1_ + i2 * b2_ + i3];
  }
};

template<class SemiLattice>
class SparseTable2d {
 public:
  using T = typename SemiLattice::T;

  int nrow_, ncol_;
  Array4d<T> table_;

 public:
  SparseTable2d(const std::vector<std::vector<T>> &v)
      : nrow_((int) v.size()),
        ncol_((int) v[0].size()),
        table_(log_base2(nrow_) + 1, log_base2(ncol_) + 1, nrow_, ncol_) {
    for (int i = 0; i < nrow_; ++i) {
      for (int j = 0; j < ncol_; ++j) {
        table_.get(0, 0, i, j) = v[i][j];
      }
    }
    for (int k = 1; (1 << k) <= nrow_; ++k) {
      for (int i = 0; i + (1 << k) <= nrow_; ++i) {
        for (int j = 0; j < ncol_; ++j) {
          table_.get(k, 0, i, j) = SemiLattice::op(
              table_.get(k - 1, 0, i, j), table_.get(k - 1, 0, i + (1 << (k - 1)), j));
        }
      }
    }
    for (int k = 0; (1 << k) <= nrow_; ++k) {
      for (int l = 1; (1 << l) <= ncol_; ++l) {
        for (int i = 0; i + (1 << k) <= nrow_; ++i) {
          for (int j = 0; j + (1 << l) <= ncol_; ++j) {
            table_.get(k, l, i, j) = SemiLattice::op(
                table_.get(k, l - 1, i, j), table_.get(k, l - 1, i, j + (1 << (l - 1))));
          }
        }
      }
    }
  }

  T fold(int x_lo, int x_hi, int y_lo, int y_hi) const {
    const int kx = log_base2(x_hi - x_lo);
    const int ky = log_base2(y_hi - y_lo);
    return SemiLattice::op({table_.get(kx, ky, x_lo, y_lo),
                            table_.get(kx, ky, x_hi - (1 << kx), y_lo),
                            table_.get(kx, ky, x_lo, y_hi - (1 << ky)),
                            table_.get(kx, ky, x_hi - (1 << kx), y_hi - (1 << ky))});
  }

 private:
  static inline int log_base2(int x) {
    return (x <= 0) ? 0 : (std::numeric_limits<unsigned>::digits - __builtin_clz((unsigned) x) - 1);
  }
};

struct MaxOp {
  using T = int16_t;
  static inline T op(const T &x, const T &y) { return std::max(x, y); }
  static inline T op(std::initializer_list<T> args) { return std::max(args); }
};

// Binary search over integers
template<class T, class F>
auto bisect(T truthy, T falsy, F pred) -> T {
  static_assert(std::is_integral_v<T>);
  static_assert(std::is_invocable_r_v<bool, F, T>);
  while (std::max(truthy, falsy) - std::min(truthy, falsy) > 1) {
    T mid = (truthy & falsy) + ((truthy ^ falsy) >> 1);
    if (pred(mid)) {
      truthy = std::move(mid);
    } else {
      falsy = std::move(mid);
    }
  }
  return truthy;
}

using namespace std;

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  const int n = in, m = in;
  vector g(n, vector(m, (int16_t) 0));
  REP(i, n) REP(j, m) cin >> g[i][j];
  vector gs(n, vector(m, (int16_t) 0));
  for (int i = n - 1; i >= 0; --i) {
    for (int j = m - 1; j >= 0; --j) {
      int s = g[i][j];
      if (s == 1 and i < n - 1 and j < m - 1) {
        s = std::min({gs[i][j + 1], gs[i + 1][j], gs[i + 1][j + 1]}) + 1;
      }
      gs[i][j] = s;
    }
  }
  SparseTable2d<MaxOp> st(gs);
  const int t = in;
  REP(i, t) {
    int x1 = in, y1 = in, x2 = in, y2 = in;
    --x1, --y1;
    int nrow = x2 - x1;
    int ncol = y2 - y1;
    int res = bisect<int>(0, std::min(nrow, ncol) + 1, [&](int k) {
      int xk = x2 - k + 1;
      int yk = y2 - k + 1;
      if (xk <= x1 or yk <= y1) return false;
      return st.fold(x1, xk, y1, yk) >= k;
    });
    print(res);
  }
}
