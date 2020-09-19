template <typename Monoid>
struct Doubling {
  using T = typename Monoid::T;
  static const int kBits = 60;
  std::vector<std::vector<int>> next_pos;
  std::vector<std::vector<T>> acc_value;

  Doubling(int n)
      : next_pos(kBits, std::vector<int>(n, -1)),
        acc_value(kBits, std::vector<T>(n, Monoid::unity())) {}

  void set_next(int i, int x) { next_pos[0][i] = x; }

  void set_value(int i, T x) { acc_value[0][i] = x; }

  void build() {
    for (int d = 0; d + 1 < kBits; d++) {
      for (size_t i = 0; i < next_pos[d].size(); i++) {
        if (int p = next_pos[d][i]; p != -1) {
          next_pos[d + 1][i] = next_pos[d][p];
          acc_value[d + 1][i] = Monoid::op(acc_value[d][i], acc_value[d][p]);
        }
      }
    }
  }

  // Folds values in [start, start + k).
  // Starting from `start`, accumulates values in `k` steps.
  std::pair<int, T> query(int start, const long long k) {
    // Only k < 2^kBits is supported.
    assert(k < (1LL << kBits));
    T res = Monoid::unity();
    int i = start;
    for (int d = kBits - 1; d >= 0; d--) {
      if ((k >> d) & 1) {
        res = Monoid::op(res, acc_value[d][i]);
        i = next_pos[d][i];
      }
    }
    return {i, res};
  }
};

//
// Monoids
//

struct UpdateOp {
  using T = int;
  static T unity() { return -1; }
  static T op(const T &x, const T &y) { return y == unity() ? x : y; }
};

struct AddOp {
  using T = long long;
  static T unity() { return 0; }
  static T op(const T &x, const T &y) { return x + y; }
};
