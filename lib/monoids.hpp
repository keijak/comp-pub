struct MinOp {
  using T = int;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

struct MaxOp {
  using T = int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::min(); }
};

struct AddOp {
  using T = int;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
};

struct MulOp {
  using T = int;
  static T op(const T &x, const T &y) { return x * y; }
  static constexpr T id() { return 1; }
};

// x := y
// Override value by the second argument.
struct AssignOp {
  using T = std::optional<int>;
  static T op(const T &x, const T &y) { return y.has_value() ? y : x; }
  static constexpr T id() { return std::nullopt; }
};

struct GCDOp {
  using T = int;
  static T op(const T &x, const T &y) {
    if (x == id()) return y;
    if (y == id()) return x;
    return std::gcd(x, y);
  }
  static constexpr T id() { return -1; }
};
