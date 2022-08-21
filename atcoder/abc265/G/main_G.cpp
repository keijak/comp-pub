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

 private:
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

struct Inv012 {
  struct T {
    array<int, 3> c;
    array<Int, 9> p;
    constexpr T() : c{0}, p{0} {}

    constexpr T(int j) : c{0}, p{0} {
      c[j] = 1;
    }

    friend ostream &operator<<(ostream &os, const T &t) {
      os << "c{" << t.c[0] << ' ' << t.c[1] << ' ' << t.c[2] << "}";
      os << " p{";
      REP(i, 9) os << t.p[i] << ' ';
      os << "}\n";
      return os;
    }
  };
  using F = array<int, 3>;

  static T op(const T &x, const T &y) {
    T res;
    res.c[0] = x.c[0] + y.c[0];
    res.c[1] = x.c[1] + y.c[1];
    res.c[2] = x.c[2] + y.c[2];
    REP(i, 9) {
      res.p[i] += x.p[i] + y.p[i];
    }
    REP(i, 3) REP(j, 3) {
        if (i == j) continue;
        int w = i * 3 + j;
        res.p[w] += x.c[i] * y.c[j];
      }
    return res;
  }
  static constexpr T id() {
    return T{};
  }

  static T f_apply(F f, const T &x) {
    T res{};
    res.c[f[0]] += x.c[0];
    res.c[f[1]] += x.c[1];
    res.c[f[2]] += x.c[2];
    REP(i, 3) REP(j, 3) {
        if (i == j or f[i] == f[j]) continue;
        int w = i * 3 + j;
        int w2 = f[i] * 3 + f[j];
        res.p[w2] += x.p[w];
      }
    return res;
  }
  static F f_compose(const F &f, const F &g) {
    F res;
    res[0] = f[g[0]];
    res[1] = f[g[1]];
    res[2] = f[g[2]];
    return res;
  }
  static constexpr F f_id() { return {0, 1, 2}; }
};

auto solve() {
  int n = in, Q = in;
  vector<int> a = in(n);
  vector<Inv012::T> init;
  init.reserve(n);
  REP(i, n) init.emplace_back(a[i]);
  LazySegmentTree<Inv012> seg(init);
  REP(qi, Q) {
    const int qt = in;
    const int L = int(in) - 1, R = int(in);
    if (qt == 1) {
      const auto res = seg.fold(L, R);
      Int inv = res.p[1 * 3 + 0] + res.p[2 * 3 + 0] + res.p[2 * 3 + 1];
      out(inv);
    } else {
      int S = in, T = in, U = in;
      seg.apply(L, R, Inv012::F{S, T, U});
    }
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
