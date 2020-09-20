struct AddOp {
  using T = int;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T unity() { return 0; }
};

struct MulOp {
  using T = int;
  static T op(const T &x, const T &y) { return x * y; }
  static constexpr T unity() { return 1; }
};

struct MinOp {
  using T = int;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T unity() { return std::numeric_limits<T>::max(); }
};

struct MaxOp {
  using T = int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T unity() { return std::numeric_limits<T>::min(); }
};

struct GCDOp {
  using T = int;
  static T op(const T &x, const T &y) {
    if (x == unity()) return y;
    if (y == unity()) return x;
    return std::gcd(x, y);
  }
  static constexpr T unity() { return -1; }
};
