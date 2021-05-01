#include <algorithm>
//#include <cassert>
#include <cstdio>
#include <vector>

template <typename LazyMonoid>
struct LazySegTree {
  using T = typename LazyMonoid::T;
  using F = typename LazyMonoid::F;

  inline int n() const { return n_; }
  inline int offset() const { return offset_; }

  explicit LazySegTree(int n)
      : LazySegTree(std::vector<T>(n, LazyMonoid::id())) {}

  explicit LazySegTree(const std::vector<T> &v) : n_(int(v.size())) {
    offset_ = 1;
    for (bits_ = 0; offset_ < n_; ++bits_) {
      offset_ <<= 1;
    }
    data_.assign(2 * offset_, LazyMonoid::id());
    lazy_ = std::vector<F>(offset_, LazyMonoid::f_id());
    for (int i = 0; i < n_; i++) data_[offset_ + i] = v[i];
    for (int i = offset_ - 1; i >= 1; i--) {
      update(i);
    }
  }

  void set(int p, T x) {
    assert(0 <= p && p < n_);
    p += offset_;
    // Update the leaf.
    for (int i = bits_; i >= 1; i--) push(p >> i);
    data_[p] = x;
    // Update its ancestors.
    for (int i = 1; i <= bits_; i++) update(p >> i);
  }

  const T &operator[](int p) const {
    assert(0 <= p && p < n_);
    p += offset_;
    for (int i = bits_; i >= 1; i--) push(p >> i);
    return data_[p];
  }

  T fold(int l, int r) const {
    assert(0 <= l && l <= r && r <= n_);
    if (l == r) return LazyMonoid::id();

    l += offset_;
    r += offset_;

    for (int i = bits_; i >= 1; i--) {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push(r >> i);
    }

    T sml = LazyMonoid::id(), smr = LazyMonoid::id();
    while (l < r) {
      if (l & 1) sml = LazyMonoid::op(sml, data_[l++]);
      if (r & 1) smr = LazyMonoid::op(data_[--r], smr);
      l >>= 1;
      r >>= 1;
    }

    return LazyMonoid::op(sml, smr);
  }

  T fold_all() const { return data_[1]; }

  void apply(int p, F f) {
    assert(0 <= p && p < n_);
    p += offset_;
    for (int i = bits_; i >= 1; i--) push(p >> i);
    data_[p] = LazyMonoid::apply(f, data_[p]);
    for (int i = 1; i <= bits_; i++) update(p >> i);
  }
  void apply(int l, int r, F f) {
    assert(0 <= l && l <= r && r <= n_);
    if (l == r) return;

    l += offset_;
    r += offset_;

    for (int i = bits_; i >= 1; i--) {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push((r - 1) >> i);
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
      if (((l >> i) << i) != l) update(l >> i);
      if (((r >> i) << i) != r) update((r - 1) >> i);
    }
  }

 private:
  void update(int k) {
    data_[k] = LazyMonoid::op(data_[2 * k], data_[2 * k + 1]);
  }
  void apply_all(int k, F f) const {
    data_[k] = LazyMonoid::f_apply(f, data_[k]);
    if (k < offset_) lazy_[k] = LazyMonoid::f_compose(f, lazy_[k]);
  }
  void push(int k) const {
    apply_all(2 * k, lazy_[k]);
    apply_all(2 * k + 1, lazy_[k]);
    lazy_[k] = LazyMonoid::f_id();
  }

  int n_, offset_, bits_;
  mutable std::vector<T> data_;
  mutable std::vector<F> lazy_;
};

struct AssignMax {
  using T = int;
  using F = int;

  // Fold: Max
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return 0; }

  // Update: Assign
  static T f_apply(const F &f, const T &x) { return (f != -1) ? f : x; }
  static F f_compose(const F &f, const F &g) { return (f != -1) ? f : g; }
  static constexpr F f_id() { return -1; }
};

using namespace std;

int main() {
  int W, N;
  scanf("%d%d", &W, &N);
  LazySegTree<AssignMax> seg(vector<int>(W + 1, 0));
  for (int i = 0; i < N; ++i) {
    int l, r;
    scanf("%d%d", &l, &r);
    --l;
    const int h = seg.fold(l, r) + 1;
    printf("%d\n", h);
    seg.apply(l, r, h);
  }
}
