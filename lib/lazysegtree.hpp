
template <typename OpMonoid>
struct LazySegTree {
  using T = typename OpMonoid::T;
  using F = typename OpMonoid::F;

  inline int n() const { return n_; }
  inline int offset() const { return offset_; }

  explicit LazySegTree(int n)
      : LazySegTree(std::vector<T>(n, OpMonoid::id())) {}

  explicit LazySegTree(const std::vector<T> &v) : n_(int(v.size())) {
    offset_ = 1;
    for (bits_ = 0; offset_ < n_; ++bits_) {
      offset_ <<= 1;
    }
    data_.assign(2 * offset_, OpMonoid::id());
    lazy_ = std::vector<F>(offset_, OpMonoid::id_f());
    for (int i = 0; i < n_; i++) data_[offset_ + i] = v[i];
    for (int i = offset_ - 1; i >= 1; i--) {
      update(i);
    }
  }

  void set(int p, T x) {
    assert(0 <= p && p < n_);
    p += offset_;
    for (int i = bits_; i >= 1; i--) push(p >> i);
    data_[p] = x;
    for (int i = 1; i <= bits_; i++) update(p >> i);
  }

  T &operator[](int p) {
    assert(0 <= p && p < n_);
    p += offset_;
    for (int i = bits_; i >= 1; i--) push(p >> i);
    return data_[p];
  }

  T fold(int l, int r) {
    assert(0 <= l && l <= r && r <= n_);
    if (l == r) return OpMonoid::id();

    l += offset_;
    r += offset_;

    for (int i = bits_; i >= 1; i--) {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push(r >> i);
    }

    T sml = OpMonoid::id(), smr = OpMonoid::id();
    while (l < r) {
      if (l & 1) sml = OpMonoid::op(sml, data_[l++]);
      if (r & 1) smr = OpMonoid::op(data_[--r], smr);
      l >>= 1;
      r >>= 1;
    }

    return OpMonoid::op(sml, smr);
  }

  void apply(int p, F f) {
    assert(0 <= p && p < n_);
    p += offset_;
    for (int i = bits_; i >= 1; i--) push(p >> i);
    data_[p] = OpMonoid::apply(f, data_[p]);
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
        if (l & 1) all_apply(l++, f);
        if (r & 1) all_apply(--r, f);
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

  template <bool (*g)(T)>
  int max_right(int l) {
    return max_right(l, [](T x) { return g(x); });
  }
  template <class G>
  int max_right(int l, G g) {
    assert(0 <= l && l <= n_);
    assert(g(OpMonoid::id()));
    if (l == n_) return n_;
    l += offset_;
    for (int i = bits_; i >= 1; i--) push(l >> i);
    T sm = OpMonoid::id();
    do {
      while (l % 2 == 0) l >>= 1;
      if (!g(OpMonoid::op(sm, data_[l]))) {
        while (l < offset_) {
          push(l);
          l = (2 * l);
          if (g(OpMonoid::op(sm, data_[l]))) {
            sm = OpMonoid::op(sm, data_[l]);
            l++;
          }
        }
        return l - offset_;
      }
      sm = OpMonoid::op(sm, data_[l]);
      l++;
    } while ((l & -l) != l);
    return n_;
  }

  template <bool (*g)(T)>
  int min_left(int r) {
    return min_left(r, [](T x) { return g(x); });
  }
  template <class G>
  int min_left(int r, G g) {
    assert(0 <= r && r <= n_);
    assert(g(OpMonoid::id()));
    if (r == 0) return 0;
    r += offset_;
    for (int i = bits_; i >= 1; i--) push((r - 1) >> i);
    T sm = OpMonoid::id();
    do {
      r--;
      while (r > 1 && (r % 2)) r >>= 1;
      if (!g(OpMonoid::op(data_[r], sm))) {
        while (r < offset_) {
          push(r);
          r = (2 * r + 1);
          if (g(OpMonoid::op(data_[r], sm))) {
            sm = OpMonoid::op(data_[r], sm);
            r--;
          }
        }
        return r + 1 - offset_;
      }
      sm = OpMonoid::op(data_[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

 private:
  void update(int k) {
    data_[k] = OpMonoid::op(data_[2 * k], data_[2 * k + 1]);
  }
  void all_apply(int k, F f) {
    data_[k] = OpMonoid::apply(f, data_[k]);
    if (k < offset_) lazy_[k] = OpMonoid::compose(f, lazy_[k]);
  }
  void push(int k) {
    all_apply(2 * k, lazy_[k]);
    all_apply(2 * k + 1, lazy_[k]);
    lazy_[k] = OpMonoid::id_f();
  }

  int n_, offset_, bits_;
  std::vector<T> data_;
  std::vector<F> lazy_;
};
