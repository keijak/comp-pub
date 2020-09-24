struct Min {
  using T = int;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

struct Max {
  using T = int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::min(); }
};

struct Sum {
  using T = int;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
};

struct Prod {
  using T = int;
  static T op(const T &x, const T &y) { return x * y; }
  static constexpr T id() { return 1; }
};

// x := y
// Override value by the second argument.
struct Assign {
  using T = std::optional<int>;
  static T op(const T &x, const T &y) { return y.has_value() ? y : x; }
  static constexpr T id() { return std::nullopt; }
};

struct GCD {
  using T = int;
  static T op(const T &x, const T &y) {
    if (x == id()) return y;
    if (y == id()) return x;
    return std::gcd(x, y);
  }
  static constexpr T id() { return -1; }
};

struct AddMin {
  // Fold: Min
  using T = long long;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }

  // Update: Add
  using F = long long;
  static T apply(const F &f, const T &x) { return f + x; }
  static F compose(const F &f, const F &g) { return f + g; }
  static constexpr F id_f() { return 0; }
};

struct AddMax {
  // Fold: Max
  using T = int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::min(); }

  // Update: Add
  using F = long long;
  static T apply(const F &f, const T &x) { return f + x; }
  static F compose(const F &f, const F &g) { return f + g; }
  static constexpr F id_f() { return 0; }
};

struct AddSum {
  // Fold: Sum
  struct T {
    long long sum;
    int width;
  };
  static T op(const T &x, const T &y) {
    return {x.sum + y.sum, x.width + y.width};
  }
  static constexpr T id() { return {0, 0}; }

  // Update: Add
  using F = long long;
  static T apply(const F &f, const T &x) {
    return {x.sum + f * x.width, x.width};
  }
  static F compose(const F &f, const F &g) { return f + g; }
  static constexpr F id_f() { return 0; }
};

struct AssignMin {
  // Fold: Min
  using T = long long;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }

  // Update: Assign
  using F = std::optional<long long>;
  static T apply(const F &f, const T &x) {
    return f.has_value() ? f.value() : x;
  }
  static F compose(const F &f, const F &g) { return f.has_value() ? f : g; }
  static constexpr F id_f() { return std::nullopt; }
};

struct AssignMax {
  // Fold: Max
  using T = int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::min(); }

  // Update: Assign
  using F = std::optional<long long>;
  static T apply(const F &f, const T &x) {
    return f.has_value() ? f.value() : x;
  }
  static F compose(const F &f, const F &g) { return f.has_value() ? f : g; }
  static constexpr F id_f() { return std::nullopt; }
};

struct AssignSum {
  // Fold: Sum
  struct T {
    long long sum;
    int width;
  };
  static T op(const T &x, const T &y) {
    return {x.sum + y.sum, x.width + y.width};
  }
  static constexpr T id() { return {0, 0}; }

  // Update: Assign
  using F = std::optional<long long>;
  static T apply(const F &f, const T &x) {
    return f.has_value() ? {f.value() * x.width, x.width} : x;
  }
  static F compose(const F &f, const F &g) { return f.has_value() ? f : g; }
  static constexpr F id_f() { return std::nullopt; }
};
