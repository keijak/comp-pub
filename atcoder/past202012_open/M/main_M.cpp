#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
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
#define cerr if(false)std::cerr
#endif

using namespace std;

// Binary search:
//   auto ok_bound = bisect(ok, ng, [&](i64 x) -> bool { return ...; });
template<class T, class F>
auto bisect(T true_x, T false_x, F pred) -> T {
  static_assert(std::is_invocable_r_v<bool, F, T>);
  assert(std::max(true_x, false_x) <= std::numeric_limits<T>::max() / 2);
  assert(true_x >= -1 and false_x >= -1);  // need floor_div to allow negative.

  while (std::abs(true_x - false_x) > 1) {
    T mid = (true_x + false_x) >> 1;
    if (pred(mid)) {
      true_x = std::move(mid);
    } else {
      false_x = std::move(mid);
    }
  }
  return true_x;
}

template<typename Monoid>
struct SegmentTree {
  using T = typename Monoid::T;

  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_

  inline int size() const { return n_; }
  inline int offset() const { return offset_; }

  explicit SegmentTree(int n) : n_(n) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
  }

  explicit SegmentTree(const std::vector<T> &leaves) : n_(leaves.size()) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
    for (int i = 0; i < n_; ++i) {
      data_[offset_ + i] = leaves[i];
    }
    for (int i = offset_ - 1; i > 0; --i) {
      data_[i] = Monoid::op(data_[i * 2], data_[i * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void set(int i, const T &x) {
    int k = offset_ + i;
    data_[k] = x;
    // Update its ancestors.
    while (k > 1) {
      k >>= 1;
      data_[k] = Monoid::op(data_[k * 2], data_[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(int l, int r) const {
    l = std::max(l, 0) + offset_;
    r = std::min(r, offset_) + offset_;
    T vleft = Monoid::id(), vright = Monoid::id();
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = Monoid::op(vleft, data_[l++]);
      if (r & 1) vright = Monoid::op(data_[--r], vright);
    }
    return Monoid::op(vleft, vright);
  }

  T fold_all() const { return data_[1]; }

  // Returns i-th value (0-indexed).
  T operator[](int i) const { return data_[offset_ + i]; }

  friend std::ostream &operator<<(std::ostream &os, const SegmentTree &st) {
    os << "[";
    for (int i = 0; i < st.size(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }
};
struct MaxOp {
  using T = char;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return 0; }
};

auto solve() {
  int n = in;
  Int L = in;
  vector<Int> a = in(n);
  vector<Int> cum(n + 1, 0);
  REP(i, n) cum[i + 1] = cum[i] + a[i];

  Int ans = bisect<Int>(0, L + 1, [&](Int x) -> bool {
    SegmentTree<MaxOp> dp(n + 1);
    dp.set(0, 1);
    int l = 0, r = 0;
    REP(i, n) {
      Int lthres = cum[i + 1] - L;
      while (l <= i and cum[l] < lthres) ++l;
      if (l == i + 1) continue;
      Int rthres = cum[i + 1] - x;
      while (r <= i and cum[r] <= rthres) ++r;
      if (dp.fold(l, r)) {
        dp.set(i + 1, 1);
      }
    }
    return dp[n];
  });
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
