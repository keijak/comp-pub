#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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

using namespace std;

template<typename T>
struct Array5d {
  std::vector<T> data_;
  size_t b0_ = 0, b1_ = 0, b2_ = 0, b3_ = 0;

  Array5d(size_t d0, size_t d1, size_t d2, size_t d3, size_t d4)
      : data_(d0 * d1 * d2 * d3 * d4),
        b0_(d1 * d2 * d3 * d4),
        b1_(d2 * d3 * d4),
        b2_(d3 * d4),
        b3_(d4) {}
  inline T &get(size_t i0, size_t i1, size_t i2, size_t i3, size_t i4) {
    return data_[i0 * b0_ + i1 * b1_ + i2 * b2_ + i3 * b3_ + i4];
  }
  // Fills one row with the specified value.
  inline void fill(size_t i0, T val) {
    std::fill(data_.begin() + i0 * b0_, data_.begin() + (i0 + 1) * b0_, val);
  }
};

auto solve() {
  string N = in;
  int n = ssize(N);
  int m = in;
  vector<int> C = in(m);
  vector<int> rc(10, -1);
  REP(j, m) rc[C[j]] = j;
  const int kFull = (1 << m) - 1;

  Array5d<Mint> dp(2, 1 << m, 2, 2, 2);
  dp.get(0, 0, 0, 0, 0) = 1;

  REP(i, n) {
    const int curd = N[i] - '0';
    const int i0 = i & 1, i1 = 1 - i0;
    dp.fill(i1, Mint(0));
    REP(state, 1 << m) REP(smaller, 2) REP(nonzero, 2) {
          Mint cur_cnt = dp.get(i0, state, smaller, nonzero, 0);
          if (cur_cnt == 0) continue;
          Mint cur_sum = dp.get(i0, state, smaller, nonzero, 1);
          REP(d, 10) {
            bool smaller2 = smaller;
            if (not smaller and d > curd) continue;
            if (not smaller and d < curd) smaller2 = true;

            bool nonzero2 = nonzero;
            if (not nonzero and d != 0) nonzero2 = true;

            int j = rc[d];
            int state2 = state;
            if (j != -1 and (d != 0 or nonzero2)) state2 |= (1 << j);

            dp.get(i1, state2, smaller2, nonzero2, 0) += cur_cnt;
            dp.get(i1, state2, smaller2, nonzero2, 1) += cur_sum * 10 + cur_cnt * d;
          }
        }
  }
  Mint ans = 0;
  Mint cnt = 0;
  REP(smaller, 2) {
    ans += dp.get((n & 1), kFull, smaller, 1, 1);
    cnt += dp.get((n & 1), kFull, smaller, 1, 0);
  }
  DUMP(cnt);
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
