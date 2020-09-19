template <typename Monoid>
struct SegTree {
  using T = typename Monoid::T;
  int size;
  std::vector<T> dat;

  SegTree(int n) : size(n), dat(2 * n, Monoid::unity()) {}

  // Sets i-th value (0-indexed) to x for initial setup.
  // build() must be called after set() calls.
  void set(int i, const T &x) { dat[size + i] = x; }
  void build() {
    for (int k = size - 1; k > 0; --k) {
      dat[k] = Monoid::op(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void update(int i, const T &x) {
    int k = size + i;
    dat[k] = x;
    while (k > 1) {
      k >>= 1;
      dat[k] = Monoid::op(dat[k * 2], dat[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, open interval).
  T fold(int l, int r) {
    l += size;
    r += size;
    T vleft = Monoid::unity(), vright = Monoid::unity();
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = Monoid::op(vleft, dat[l++]);
      if (r & 1) vright = Monoid::op(dat[--r], vright);
    }
    return Monoid::op(vleft, vright);
  }

  // Queries by a single index (0-indexed).
  T operator[](int i) const { return dat[size + i]; }
};
template <typename T>
std::ostream &operator<<(std::ostream &os, const SegTree<T> &st) {
  os << "[";
  for (int i = 0; i < st.size; ++i) {
    if (i != 0) os << ", ";
    os << st[i];
  }
  return os << "]";
}

// Monoid

struct AddOp {
  using T = mint;
  static T op(const T &x, const T &y) { return x + y; }
  static inline T unity() { return 0; }
};

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
