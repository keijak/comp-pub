// A functional graph is a directed graph in which each vertex has outdegree
// one, and can therefore be specified by a function mapping {0,...,n-1} onto
// itself.
// https://mathworld.wolfram.com/FunctionalGraph.html
template <typename Monoid>
struct FunctionalGraphDoubling {
  using T = typename Monoid::T;
  static const int kMaxBits = 60;

  // number of nodes.
  int size;

  // acc_value[d][i] := starting from i, what's the value accumulated after 2^d
  // steps.
  std::vector<std::vector<T>> acc_value;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  explicit FunctionalGraphDoubling(int n)
      : size(n),
        next_pos(kMaxBits, std::vector<int>(n, -1)),
        acc_value(kMaxBits, std::vector<T>(n, Monoid::unity())) {}

  // Sets value `x` at node `i`.
  void set_value(int i, T x) { acc_value[0][i] = x; }

  // Sets next position of node `i`.
  void set_next(int i, int pos) { next_pos[0][i] = pos; }

  // Builds transition tables.
  void build() {
    for (int d = 0; d + 1 < kMaxBits; d++) {
      for (int i = 0; i < size; i++) {
        if (int p = next_pos[d][i]; p != -1) {
          next_pos[d + 1][i] = next_pos[d][p];
          acc_value[d + 1][i] = Monoid::op(acc_value[d][i], acc_value[d][p]);
        }
      }
    }
  }

  // Starting from `start`, `steps` times goes forward and accumulates values.
  T transition(int start, const long long steps) {
    // Only k < 2^kMaxBits is supported.
    assert(steps < (1LL << kMaxBits));
    T res = Monoid::unity();
    int i = start;
    for (int d = kMaxBits - 1; d >= 0; d--) {
      if ((steps >> d) & 1) {
        res = Monoid::op(res, acc_value[d][i]);
        i = next_pos[d][i];
      }
    }
    return res;
  }
};

// Monoid

// x := y
// Override value by the second argument.
struct AssignOp {
  using T = std::optional<int>;
  static T op(const T &x, const T &y) { return y.has_value() ? y : x; }
  static T unity() { return std::nullopt; }
};

struct AddOp {
  using T = long long;
  static T unity() { return 0; }
  static T op(const T &x, const T &y) { return x + y; }
};
