template <typename Monoid>
struct DualSegTree {
  using T = typename Monoid::T;
  int size;
  std::vector<T> dat;

  DualSegTree(int n) : size(n), dat(2 * n, Monoid::unity()) {}

  // Applies a value to [l,r) range.
  void apply(int l, int r, T val) {
    l += size;
    r += size;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) {
        dat[l] = Monoid::op(dat[l], val);
        ++l;
      }
      if (r & 1) {
        --r;
        dat[r] = Monoid::op(dat[r], val);
      }
    }
  }

  // Returns i-th value.
  T operator[](int i) const {
    int k = size + i;
    T res = dat[k];
    while (k > 1) {
      k >>= 1;
      res = Monoid::op(res, dat[k]);
    }
    return res;
  }
};
template <typename T>
std::ostream &operator<<(std::ostream &os, const DualSegTree<T> &st) {
  os << "[";
  for (int i = 0; i < st.size; ++i) {
    if (i != 0) os << ", ";
    os << st[i];
  }
  return os << "]";
}

// Monoid

struct MinOp {
  using T = int;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static inline T unity() { return std::numeric_limits<T>::max(); }
};

struct MaxOp {
  using T = int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static inline T unity() { return std::numeric_limits<T>::min(); }
};
