#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<typename Monoid>
struct SegTree {
  using T = typename Monoid::T;

  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_

  inline int size() const { return n_; }
  inline int offset() const { return offset_; }

  explicit SegTree(int n) : n_(n) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
  }

  explicit SegTree(const std::vector<T> &leaves) : n_(leaves.size()) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
    for (int i = 0; i < n_; ++i) {
      data_[offset_ + i] = leaves[i];
    }
    for (int i = offset_ - 1; i > 0; --i) {
      data_[i] = Monoid::op(data_[i * 2], data_[i * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void set(int i, const T &x) {
    int k = offset_ + i;
    data_[k] = x;
    // Update its ancestors.
    while (k > 1) {
      k >>= 1;
      data_[k] = Monoid::op(data_[k * 2], data_[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(int l, int r) const {
    l = std::max(l, 0) + offset_;
    r = std::min(r, offset_) + offset_;
    T vleft = Monoid::id(), vright = Monoid::id();
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = Monoid::op(vleft, data_[l++]);
      if (r & 1) vright = Monoid::op(data_[--r], vright);
    }
    return Monoid::op(vleft, vright);
  }

  T fold_all() const { return data_[1]; }

  // Returns i-th value (0-indexed).
  T operator[](int i) const { return data_[offset_ + i]; }

  friend std::ostream &operator<<(std::ostream &os, const SegTree &st) {
    os << "[";
    for (int i = 0; i < st.size(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }
};

struct SumOp {
  using T = long long;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
  static T invert(const T &x) { return -x; }
};

struct MinOp {
  using T = long long;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return std::numeric_limits<T>::max(); }
};

// Fix the left bound, extend the right bound as much as possible.
template<class M, class F>
int max_right(const SegTree<M> &seg, int l, F pred) {
  static_assert(std::is_invocable_r_v<bool, F, typename M::T>,
                "predicate must be invocable on the value type");
  assert(0 <= l && l <= seg.size());
  assert(pred(M::id()));
  if (l == seg.size()) return seg.size();
  l += seg.offset_;
  auto sm = M::id();
  do {
    while (l % 2 == 0) l >>= 1;
    if (not pred(M::op(sm, seg.data_[l]))) {
      while (l < seg.offset_) {
        l <<= 1;
        if (pred(M::op(sm, seg.data_[l]))) {
          sm = M::op(sm, seg.data_[l]);
          ++l;
        }
      }
      return l - seg.offset_;
    }
    sm = M::op(sm, seg.data_[l]);
    ++l;
  } while ((l & -l) != l);
  return seg.size();
}

template<typename V>
void flip_vertically(std::vector<V> &grid) {
  const int h = grid.size();
  if (h == 0) return;
  const int w = grid[0].size();
  if (w == 0) return;
  for (int i = 0;; ++i) {
    const int r = h - 1 - i;
    if (r <= i) break;
    std::swap(grid[i], grid[r]);
  }
}

auto solve() {
  const int R = in, C = in;
  int K = int(in) - 1;
  const int S = in;
  vector<string> G(R);
  cin.ignore();
  REP(i, R) {
    getline(cin, G[i]);
    assert(ssize(G[i]) == C);
  }
  vector<pair<int, int>> spells(S);
  REP(i, S) {
    int a = in, b = in;
    spells[i] = {a - 1, b - 1};
  }

  vector<i64> ans(S, C);
  vector<SegTree<MinOp>> costs;
  vector<vector<SegTree<SumOp>>> segs_x(2);
  vector<vector<set<int>>> sslots(2, vector<set<int>>(C));
  vector<vector<string>> gg(2);
  gg[0] = G;
  gg[1] = G;
  flip_vertically(gg[1]);
  vector<int> gk = {K, R - 1 - K};

  REP(flipping, 2) {
    auto &g = gg[flipping];
    int k = gk[flipping];

    vector<i64> init_cost(R + 1, C);
    REP(i, R + 1) {
      init_cost[i] = C;
    }
    REP(i, k + 1, R + 1) {
      init_cost[i] += i - k;
    }
    costs.emplace_back(init_cost);
    auto &cost = costs.back();
    auto &seg_x = segs_x[flipping];
    auto &slots = sslots[flipping];

    REP(j, C) {
      vector<i64> init_x(R, 0);
      REP(i, R) {
        if (g[i][j] == 'X') init_x[i] = 1;
      }
      seg_x.emplace_back(init_x);
    }

    REP(j, C) {
      int x_count = 0;
      REP(i, R + 1) {
        if (x_count >= k + 1) break;
        if (i == R) {
          cost.set(i, cost[i] - 1);
          break;
        }
        if (g[i][j] == 'X') {
          ++x_count;
        } else {
          slots[j].insert(i);
          cost.set(i, cost[i] - 1);
        }
      }
    }
  }

  REP(i, S) {
    REP(flipping, 2) {
      auto &cost = costs[flipping];
      auto &seg_x = segs_x[flipping];
      auto &g = gg[flipping];
      auto &slots = sslots[flipping];
      int k = gk[flipping];

      int r, c;
      tie(r, c) = spells[i];
      if (flipping) {
        r = R - 1 - r;
      }

      int r0 = max_right(seg_x[c], 0, [&](i64 x) { return x <= k; });
      if (g[r][c] == 'X') {
        seg_x[c].set(r, 0);
        g[r][c] = '.';
        slots[c].insert(r);
        if (seg_x[c].fold(0, r) <= k) {
          cost.set(r, cost[r] - 1);
          int r1 = max_right(seg_x[c], 0, [&](i64 x) { return x <= k; });
          assert(r0 <= r1);
          for (auto it = slots[c].lower_bound(r0 + 1); it != slots[c].end() and *it < r1; ++it) {
            if (*it == r) continue;
            cost.set(*it, cost[*it] - 1);
          }
        }
      } else {
        seg_x[c].set(r, 1);
        g[r][c] = 'X';
        slots[c].erase(r);
        if (seg_x[c].fold(0, r) <= k) {
          cost.set(r, cost[r] + 1);
          int r1 = max_right(seg_x[c], 0, [&](i64 x) { return x <= k; });
          assert(r1 <= r0);
          for (auto it = slots[c].lower_bound(r1 + 1); it != slots[c].end() and *it < r0; ++it) {
            if (*it == r) continue;
            cost.set(*it, cost[*it] + 1);
          }
        }
      }
      chmin(ans[i], cost.fold(k, R + 1));
    }
  }
  DUMP(ans);
  return accumulate(ALL(ans), 0LL);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = in;
  REP(test_case, t) {
    cout << "Case #" << (test_case + 1) << ": ";
    auto ans = solve();
    print(ans);
  }
}
