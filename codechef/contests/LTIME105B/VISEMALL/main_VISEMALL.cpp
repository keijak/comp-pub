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
    return std::cout << (x ? "YES" : "NO") << endc;
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

template<typename Monoids>
struct LazySegmentTree {
  using T = typename Monoids::T;
  using F = typename Monoids::F;

  inline int size() const { return n_; }
  inline int offset() const { return offset_; }

  explicit LazySegmentTree(int n)
      : LazySegmentTree(std::vector<T>(n, Monoids::id())) {}

  explicit LazySegmentTree(const std::vector<T> &v) : n_(int(v.size())) {
    offset_ = 1;
    for (bits_ = 0; offset_ < n_; ++bits_) {
      offset_ <<= 1;
    }
    data_.assign(2 * offset_, Monoids::id());
    lazy_ = std::vector<F>(offset_, Monoids::f_id());
    for (int i = 0; i < n_; i++) data_[offset_ + i] = v[i];
    for (int i = offset_ - 1; i >= 1; i--) {
      pull_up(i);
    }
  }

  void set(int p, T x) {
    assert(0 <= p && p < n_);
    p += offset_;
    // Update the leaf.
    for (int i = bits_; i >= 1; i--) push_down(p >> i);
    data_[p] = x;
    // Update its ancestors.
    for (int i = 1; i <= bits_; i++) pull_up(p >> i);
  }

  const T &operator[](int p) const {
    assert(0 <= p && p < n_);
    p += offset_;
    for (int i = bits_; i >= 1; i--) push_down(p >> i);
    return data_[p];
  }

  T fold(int l, int r) const {
    assert(0 <= l && l <= r && r <= n_);
    if (l == r) return Monoids::id();

    l += offset_;
    r += offset_;

    for (int i = bits_; i >= 1; i--) {
      if (((l >> i) << i) != l) push_down(l >> i);
      if (((r >> i) << i) != r) push_down(r >> i);
    }

    T sml = Monoids::id(), smr = Monoids::id();
    while (l < r) {
      if (l & 1) sml = Monoids::op(sml, data_[l++]);
      if (r & 1) smr = Monoids::op(data_[--r], smr);
      l >>= 1;
      r >>= 1;
    }

    return Monoids::op(sml, smr);
  }

  T fold_all() const { return data_[1]; }

  void apply(int p, F f) {
    assert(0 <= p && p < n_);
    p += offset_;
    for (int i = bits_; i >= 1; i--) push_down(p >> i);
    data_[p] = Monoids::apply(f, data_[p]);
    for (int i = 1; i <= bits_; i++) pull_up(p >> i);
  }
  void apply(int l, int r, F f) {
    assert(0 <= l && l <= r && r <= n_);
    if (l == r) return;

    l += offset_;
    r += offset_;

    for (int i = bits_; i >= 1; i--) {
      if (((l >> i) << i) != l) push_down(l >> i);
      if (((r >> i) << i) != r) push_down((r - 1) >> i);
    }
    {
      int l2 = l, r2 = r;
      while (l < r) {
        if (l & 1) apply_all(l++, f);
        if (r & 1) apply_all(--r, f);
        l >>= 1;
        r >>= 1;
      }
      l = l2;
      r = r2;
    }
    for (int i = 1; i <= bits_; i++) {
      if (((l >> i) << i) != l) pull_up(l >> i);
      if (((r >> i) << i) != r) pull_up((r - 1) >> i);
    }
  }

  std::vector<T> to_vec(int sz = -1) const {
    if (sz < 0 or sz > size()) sz = size();
    std::vector<T> res(sz);
    for (int i = 0; i < sz; ++i) res[i] = (*this)[i];
    return res;
  }

  int n_, offset_, bits_;
  mutable std::vector<T> data_;
  mutable std::vector<F> lazy_;

  void pull_up(int k) const {
    data_[k] = Monoids::op(data_[2 * k], data_[2 * k + 1]);
  }

  void push_down(int k) const {
    apply_all(2 * k, lazy_[k]);
    apply_all(2 * k + 1, lazy_[k]);
    lazy_[k] = Monoids::f_id();
  }

  void apply_all(int k, F f) const {
    data_[k] = Monoids::f_apply(f, data_[k]);
    if (k < offset_) {
      lazy_[k] = Monoids::f_compose(f, lazy_[k]);
#ifdef SEGMENT_TREE_BEATS
      if (data_[k].failed) push_down(k), pull_up(k);
#endif
    }
  }
};

template<class M, class F>
int min_left(const LazySegmentTree<M> &seg, int r, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, typename M::T>,
                "predicate must be invocable on the value type");
  assert(0 <= r && r <= seg.n_);
  assert(pred(M::id()));
  if (r == 0) return 0;
  r += seg.offset_;
  for (int i = seg.bits_; i >= 1; --i) {
    seg.push_down((r - 1) >> i);
  }
  auto sm = M::id();
  do {
    --r;
    while (r > 1 && (r % 2)) r >>= 1;
    if (not pred(M::op(seg.data_[r], sm))) {
      while (r < seg.offset_) {
        seg.push_down(r);
        r = 2 * r + 1;
        if (pred(M::op(seg.data_[r], sm))) {
          sm = M::op(seg.data_[r], sm);
          --r;
        }
      }
      return r + 1 - seg.offset_;
    }
    sm = M::op(seg.data_[r], sm);
  } while ((r & -r) != r);
  return 0;
}

// Range Flip Range Sum over 0-1 sequence.
struct XorSumOp {
  struct T {
    int ones;
    int width;  // NOTE: Must be initialized with width=1!
  };
  using F = int;

  // Fold: Sum
  static T op(const T &x, const T &y) {
    return {x.ones + y.ones, x.width + y.width};
  }
  static T id() { return {0, 0}; }

  // Update: 0-1 flip by f=1.
  static T f_apply(const F &f, const T &x) {
    if (f == 0) return x;
    return {x.width - x.ones, x.width};
  }
  static F f_compose(const F &f, const F &g) { return f ^ g; }
  static constexpr F f_id() { return 0; }
};

auto solve() {
  int n = in, Q = in;
  string S = in;
  assert(ssize(S) == n);

  vector<XorSumOp::T> seg_init(n);
  REP(i, n) {
    int val = (S[i] == '1') ? 1 : 0;
    seg_init[i] = {val, 1};
  }
  LazySegmentTree<XorSumOp> seg(seg_init);

  REP(qi, Q) {
    int qtype = in, L = in, R = in;
    --L;
    if (qtype == 1) {
      seg.apply(L, R, 1);
      continue;
    }
    const int X = int(in) - 1;
    bool all_inv = seg[X].ones == 1;
    if (all_inv) {
      seg.apply(0, n, 1);
    }

    auto query = [&]() -> bool {
      if (R - L <= 1) return true;
      if (seg[R - 1].ones == 1) {
        // Rend==1
        if (seg[L].ones == 1) {
          return true;
        }
        int rmost0 = min_left(seg, R, [&](const XorSumOp::T &val) {
          return val.ones == val.width;
        });
        --rmost0;
        return seg.fold(L, rmost0).ones > 0;
      } else {
        // Rend==0
        if (X == L) return true;
        if (seg[L].ones == 0) {
          return seg.fold(L, R).ones > 0;
        }
        // Lend==1, Rend==0
        if (auto lagg = seg.fold(L, X); lagg.ones == lagg.width) {
          return true;
        }
        int rmost1 = min_left(seg, R, [&](const XorSumOp::T &val) {
          return val.ones == 0;
        });
        --rmost1;
        if (auto lagg = seg.fold(L, rmost1); lagg.ones < lagg.width) {
          return true;
        }
        return rmost1 >= X - 1;
      }
    };
    print(query());
    if (all_inv) {
      seg.apply(0, n, 1);
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
