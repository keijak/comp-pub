// #define NDEBUG
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
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

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

  std::vector<T> to_vec(int sz = -1) const {
    if (sz < 0 or sz > size()) sz = size();
    std::vector<T> res(sz);
    for (int i = 0; i < sz; ++i) res[i] = (*this)[i];
    return res;
  }
};
struct MaxOp {
  using T = long long;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::lowest(); }
};

struct MinOp {
  using T = long long;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

auto solve() {
  int n = in, m = in;
  vector<int> A(n), B(n);
  vector<vector<int>> alist(m + 1), blist(m + 1);
  REP(i, n) {
    cin >> A[i] >> B[i];
    assert(A[i] < B[i]);
    alist[A[i]].push_back(i);
    blist[B[i]].push_back(i);
  }
  vector<Int> ans(m + 100);

  SegmentTree<MinOp> seg(vector<Int>(n, -1));
  for (int r = 1; r <= m; ++r) {
    for (int j: alist[r]) {
      seg.set(j, r);
    }
    for (int j: blist[r]) {
      seg.set(j, r);
    }
    int minval = seg.fold_all();
    if (minval >= 0) {
      int len_min = max(r - minval + 1, 1);
      int len_max = r;
      ans[len_min] += 1;
      ans[len_max + 1] -= 1;
    }
  }
  for (int i = 1; i <= m; ++i) {
    ans[i] += ans[i - 1];
    cout << ans[i] << (i == m ? "\n" : " ");
  }
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}
