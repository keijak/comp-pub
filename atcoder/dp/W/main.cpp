#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

template <typename Monoid, typename OperatorMonoid = Monoid>
struct LazySegmentTree {
  using AggFn = function<Monoid(Monoid, Monoid)>;
  using UpdateFn = function<OperatorMonoid(OperatorMonoid, OperatorMonoid)>;
  using CombineFn = function<Monoid(Monoid, OperatorMonoid)>;

  int sz, height;
  vector<Monoid> data;
  vector<OperatorMonoid> lazy;
  const AggFn agg_;          // aggregator
  const UpdateFn update_;    // updater
  const CombineFn combine_;  // combiner
  const Monoid M1;
  const OperatorMonoid OM0;

  LazySegmentTree(int n, const AggFn f, const UpdateFn g, const CombineFn h,
                  const Monoid &M1, const OperatorMonoid OM0)
      : agg_(f), update_(g), combine_(h), M1(M1), OM0(OM0) {
    sz = 1;
    height = 0;
    while (sz < n) sz <<= 1, height++;
    data.assign(2 * sz, M1);
    lazy.assign(2 * sz, OM0);
  }

  void set(int k, const Monoid &x) { data[k + sz] = x; }

  void build() {
    for (int k = sz - 1; k > 0; k--) {
      data[k] = agg_(data[2 * k + 0], data[2 * k + 1]);
    }
  }

  inline void propagate(int k) {
    if (lazy[k] != OM0) {
      lazy[2 * k + 0] = update_(lazy[2 * k + 0], lazy[k]);
      lazy[2 * k + 1] = update_(lazy[2 * k + 1], lazy[k]);
      data[k] = apply(k);
      lazy[k] = OM0;
    }
  }

  inline Monoid apply(int k) {
    return lazy[k] == OM0 ? data[k] : combine_(data[k], lazy[k]);
  }

  inline void recalc(int k) {
    while (k >>= 1) data[k] = agg_(apply(2 * k + 0), apply(2 * k + 1));
  }

  inline void thrust(int k) {
    for (int i = height; i > 0; i--) propagate(k >> i);
  }

  void update(int a, int b, const OperatorMonoid &x) {
    if (a >= b) return;
    thrust(a += sz);
    thrust(b += sz - 1);
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) lazy[l] = update_(lazy[l], x), ++l;
      if (r & 1) --r, lazy[r] = update_(lazy[r], x);
    }
    recalc(a);
    recalc(b);
  }

  Monoid fold(int a, int b) {
    if (a >= b) return M1;
    thrust(a += sz);
    thrust(b += sz - 1);
    Monoid L = M1, R = M1;
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) L = agg_(L, apply(l++));
      if (r & 1) R = agg_(apply(--r), R);
    }
    return agg_(L, R);
  }

  Monoid operator[](const int &k) { return fold(k, k + 1); }

  template <typename C>
  int find_subtree(int a, const C &check, Monoid &M, bool type) {
    while (a < sz) {
      propagate(a);
      Monoid nxt =
          type ? agg_(apply(2 * a + type), M) : agg_(M, apply(2 * a + type));
      if (check(nxt))
        a = 2 * a + type;
      else
        M = nxt, a = 2 * a + 1 - type;
    }
    return a - sz;
  }

  template <typename C>
  int find_first(int a, const C &check) {
    Monoid L = M1;
    if (a <= 0) {
      if (check(agg_(L, apply(1)))) return find_subtree(1, check, L, false);
      return -1;
    }
    thrust(a + sz);
    int b = sz;
    for (a += sz, b += sz; a < b; a >>= 1, b >>= 1) {
      if (a & 1) {
        Monoid nxt = agg_(L, apply(a));
        if (check(nxt)) return find_subtree(a, check, L, false);
        L = nxt;
        ++a;
      }
    }
    return -1;
  }

  template <typename C>
  int find_last(int b, const C &check) {
    Monoid R = M1;
    if (b >= sz) {
      if (check(agg_(apply(1), R))) return find_subtree(1, check, R, true);
      return -1;
    }
    thrust(b + sz - 1);
    int a = sz;
    for (b += sz; a < b; a >>= 1, b >>= 1) {
      if (b & 1) {
        Monoid nxt = agg_(apply(--b), R);
        if (check(nxt)) return find_subtree(b, check, R, true);
        R = nxt;
      }
    }
    return -1;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vector<tuple<int, int, i64>> intervals(m);
  REP(i, m) {
    i64 l, r, a;
    cin >> l >> r >> a;
    --l;
    --r;
    intervals[i] = {r, l, a};
  }
  sort(intervals.begin(), intervals.end());
  LazySegmentTree<i64> st(n, ([](i64 x, i64 y) { return max(x, y); }),
                          ([](i64 x, i64 y) { return x + y; }),
                          ([](i64 x, i64 y) { return x + y; }), 0LL, 0LL);

  int j = 0;
  for (int i = 0; i < n; ++i) {
    i64 pmax = st.fold(0, i);
    i64 asum = 0;
    for (; j < int(intervals.size()); ++j) {
      auto [r, l, a] = intervals[j];
      if (r > i) break;
      assert(r == i);
      asum += a;
      st.update(l, i, a);
    }
    st.update(i, i + 1, pmax + asum);
  }
  cout << st.fold(0, n) << endl;
}
