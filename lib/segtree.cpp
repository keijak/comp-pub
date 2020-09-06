template <class Monoid>
struct SegTree {
  using Func = function<Monoid(Monoid, Monoid)>;
  const Func F;
  const Monoid UNITY;
  int size;
  vector<Monoid> dat;

  SegTree(int n, const Func f, const Monoid &unity)
      : F(f), UNITY(unity), size(n), dat(2 * n, unity) {}

  // Sets i-th value (0-indexed) to x for initial setup.
  // build() must be called after set() calls.
  void set(int i, const Monoid &x) { dat[size + i] = x; }
  void build() {
    for (int k = size - 1; k > 0; --k) {
      dat[k] = F(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void update(int i, const Monoid &x) {
    int k = size + i;
    dat[k] = x;
    while (k > 1) {
      k >>= 1;
      dat[k] = F(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, open interval).
  Monoid fold(int l, int r) {
    l += size;
    r += size;
    Monoid vleft = UNITY, vright = UNITY;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = F(vleft, dat[l++]);
      if (r & 1) vright = F(dat[--r], vright);
    }
    return F(vleft, vright);
  }

  // Queries by a single index (0-indexed).
  Monoid operator[](int i) const { return dat[size + i]; }
};
template <typename T>
ostream &operator<<(ostream &os, const SegTree<T> &st) {
  os << "[";
  for (int i = 0; i < st.size; ++i) {
    if (i != 0) os << ", ";
    os << st[i];
  }
  return os << "]";
}
