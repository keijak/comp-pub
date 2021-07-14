#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

constexpr int kMaxColor = 100005;
vector<int> kNodeColors;

// tour: preorder node ids
// The interval [in[v], out[v]) represents a subtree whose
// root is v.
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G g;

  // Euler Tour on nodes.
  vector<int> tour;  // color
  vector<int> in;    // index in the tour on entering the subtree of v
  vector<int> out;   // index in the tour on exiting the subtree of v
  vector<int> depth;

  explicit EulerTour(G g, int root = 0)
      : n((int)g.size()), g(move(g)), in(n, -1), out(n, -1), depth(n, -1) {
    tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    in[v] = int(tour.size());
    if (p >= 0) depth[v] = depth[p] + 1;
    tour.emplace_back(kNodeColors[v]);
    for (auto u : g[v]) {
      if (u == p) continue;
      dfs(u, v);
    }
    out[v] = int(tour.size());
  }
};

template <typename Monoid>
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

struct Sum {
  using T = int;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
};

struct Query {
  int idx;
  int l;
  int r;
  int k;
};

struct Task {
  vector<int> tour;
  vector<int> ans;
  vector<int> freq;
  SegTree<Sum> seg;

  explicit Task(const vector<int> &tour, int m)
      : tour(tour), ans(m), freq(kMaxColor + 1, 0), seg(ssize(tour)) {
    seg.set(0, ssize(tour));
  }

  void add(int i) {
    int c = tour[i];
    seg.set(freq[c], seg[freq[c]] - 1);
    ++freq[c];
    seg.set(freq[c], seg[freq[c]] + 1);
  }
  void erase(int i) {
    int c = tour[i];
    seg.set(freq[c], seg[freq[c]] - 1);
    --freq[c];
    seg.set(freq[c], seg[freq[c]] + 1);
  }
  void add_left(int l) { add(l); }
  void add_right(int r) { add(r); }
  void erase_left(int l) { erase(l); }
  void erase_right(int r) { erase(r); }
  void answer(const Query &q) { ans[q.idx] = seg.fold(q.k, ssize(tour) + 1); }
};

struct Mo {
  int n;
  vector<Query> queries;

  explicit Mo(int n) : n(n) {}

  void add_query(int query_index, int l, int r, int k) { /* [l, r) */
    queries.push_back(Query{query_index, l, r, k});
  }

  void solve(Task &task) {
    int q = (int)queries.size();
    int bs = n / min<int>(n, sqrt(q));
    vector<int> ord(q);
    iota(begin(ord), end(ord), 0);
    sort(begin(ord), end(ord), [&](int a, int b) {
      int ablock = queries[a].l / bs, bblock = queries[b].l / bs;
      if (ablock != bblock) return ablock < bblock;
      return (ablock & 1) ? queries[a].r > queries[b].r
                          : queries[a].r < queries[b].r;
    });
    int l = 0, r = 0;
    for (auto idx : ord) {
      while (l > queries[idx].l) task.add_left(--l);
      while (r < queries[idx].r) task.add_right(r++);
      while (l < queries[idx].l) task.erase_left(l++);
      while (r > queries[idx].r) task.erase_right(--r);
      task.answer(queries[idx]);
    }
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  kNodeColors.resize(n);
  REP(i, n) cin >> kNodeColors[i];
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  EulerTour et(g);
  assert(ssize(et.tour) == n);
  Mo mo(et.tour.size());

  REP(i, m) {
    int v, k;
    cin >> v >> k;
    --v;
    mo.add_query(i, et.in[v], et.out[v], k);
  }
  Task task(et.tour, m);
  mo.solve(task);
  REP(i, m) { cout << task.ans[i] << "\n"; }
}
