template <typename Monoid, typename OperatorMonoid = Monoid>
struct LazySegmentTree {
  using AggFn = function<Monoid(Monoid, Monoid)>;
  using UpdateFn = function<OperatorMonoid(OperatorMonoid, OperatorMonoid)>;
  using CombineFn = function<Monoid(Monoid, OperatorMonoid)>;

  int size, height;
  vector<Monoid> data;
  vector<OperatorMonoid> lazy;
  const AggFn agg_;          // aggregator
  const UpdateFn update_;    // updater
  const CombineFn combine_;  // combiner
  const Monoid M1;
  const OperatorMonoid OM0;

  LazySegmentTree(int n, const AggFn f, const UpdateFn g, const CombineFn h,
                  const Monoid &M1, const OperatorMonoid OM0)
      : agg_(f), update_(g), combine_(h), M1(M1), OM0(OM0) {
    size = 1;
    height = 0;
    while (size < n) size <<= 1, height++;
    data.assign(2 * size, M1);
    lazy.assign(2 * size, OM0);
  }

  void set(int k, const Monoid &x) { data[k + size] = x; }

  void build() {
    for (int k = size - 1; k > 0; k--) {
      data[k] = agg_(data[2 * k + 0], data[2 * k + 1]);
    }
  }

  inline void propagate(int k) {
    if (lazy[k] != OM0) {
      lazy[2 * k + 0] = update_(lazy[2 * k + 0], lazy[k]);
      lazy[2 * k + 1] = update_(lazy[2 * k + 1], lazy[k]);
      data[k] = apply(k);
      lazy[k] = OM0;
    }
  }

  inline Monoid apply(int k) {
    return lazy[k] == OM0 ? data[k] : combine_(data[k], lazy[k]);
  }

  inline void recalc(int k) {
    while (k >>= 1) data[k] = agg_(apply(2 * k + 0), apply(2 * k + 1));
  }

  inline void thrust(int k) {
    for (int i = height; i > 0; i--) propagate(k >> i);
  }

  void update(int a, int b, const OperatorMonoid &x) {
    if (a >= b) return;
    thrust(a += size);
    thrust(b += size - 1);
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) lazy[l] = update_(lazy[l], x), ++l;
      if (r & 1) --r, lazy[r] = update_(lazy[r], x);
    }
    recalc(a);
    recalc(b);
  }

  Monoid fold(int a, int b) {
    if (a >= b) return M1;
    thrust(a += size);
    thrust(b += size - 1);
    Monoid L = M1, R = M1;
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) L = agg_(L, apply(l++));
      if (r & 1) R = agg_(apply(--r), R);
    }
    return agg_(L, R);
  }

  Monoid operator[](const int &k) { return fold(k, k + 1); }

  template <typename C>
  int find_subtree(int a, const C &check, Monoid &M, bool type) {
    while (a < size) {
      propagate(a);
      Monoid nxt =
          type ? agg_(apply(2 * a + type), M) : agg_(M, apply(2 * a + type));
      if (check(nxt))
        a = 2 * a + type;
      else
        M = nxt, a = 2 * a + 1 - type;
    }
    return a - size;
  }

  template <typename C>
  int find_first(int a, const C &check) {
    Monoid L = M1;
    if (a <= 0) {
      if (check(agg_(L, apply(1)))) return find_subtree(1, check, L, false);
      return -1;
    }
    thrust(a + size);
    int b = size;
    for (a += size, b += size; a < b; a >>= 1, b >>= 1) {
      if (a & 1) {
        Monoid nxt = agg_(L, apply(a));
        if (check(nxt)) return find_subtree(a, check, L, false);
        L = nxt;
        ++a;
      }
    }
    return -1;
  }

  template <typename C>
  int find_last(int b, const C &check) {
    Monoid R = M1;
    if (b >= size) {
      if (check(agg_(apply(1), R))) return find_subtree(1, check, R, true);
      return -1;
    }
    thrust(b + size - 1);
    int a = size;
    for (b += size; a < b; a >>= 1, b >>= 1) {
      if (b & 1) {
        Monoid nxt = agg_(apply(--b), R);
        if (check(nxt)) return find_subtree(b, check, R, true);
        R = nxt;
      }
    }
    return -1;
  }
};
