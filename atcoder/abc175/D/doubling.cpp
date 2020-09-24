#include <bits/stdc++.h>
using i64 = std::int64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}

// A functional graph is a directed graph in which each vertex has outdegree
// one, and can therefore be specified by a function mapping {0,...,n-1} onto
// itself.
// https://mathworld.wolfram.com/FunctionalGraph.html
template <typename Monoid>
struct FunctionalGraph {
  using T = typename Monoid::T;
  static const int kMaxBits = 32;

  // number of nodes.
  int size;

  // acc_value[d][i] := starting from i, what's the value accumulated after 2^d
  // steps.
  std::vector<std::vector<T>> acc_value;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  explicit FunctionalGraph(int n)
      : size(n),
        acc_value(kMaxBits, std::vector<T>(n, Monoid::id())),
        next_pos(kMaxBits, std::vector<int>(n, -1)) {}

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
  T transition(int start, const long long steps) const {
    // steps >= 2^kMaxBits is not supported.
    assert(steps < (1LL << kMaxBits));
    T res = Monoid::id();
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

const i64 INF = 1e18;

struct MaxSumOp {
  using T = std::pair<i64, i64>;
  static T op(const T &x, const T &y) {
    auto [m1, s1] = x;
    auto [m2, s2] = y;
    if (m2 == -INF) {
      return {m1, s1 + s2};
    } else {
      return {std::max(m1, s1 + m2), s1 + s2};
    }
  }
  static constexpr T id() { return {-INF, 0LL}; }
};

using namespace std;

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  V<int> P(n), C(n);
  cin >> P >> C;
  FunctionalGraph<MaxSumOp> g(n);
  REP(i, n) {
    int nx = P[i] - 1;
    g.set_next(i, nx);
    g.set_value(i, {C[nx], C[nx]});
  }
  g.build();
  i64 ans = -INF;
  REP(i, n) { chmax(ans, g.transition(i, k).first); }
  cout << ans << endl;
}
