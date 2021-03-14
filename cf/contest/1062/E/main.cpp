#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

// tour: preorder node ids
// The interval [pre_index[v], post_index[v]) represents a subtree whose
// root is v.
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G adj;

  // Euler Tour on nodes.
  vector<int> tour;        // nodes
  vector<int> pre_index;   // index in the tour on entering the subtree of v
  vector<int> post_index;  // index in the tour on exiting the subtree of v
  vector<int> level;

  explicit EulerTour(G g, int root = 0)
      : n((int)g.size()),
        adj(move(g)),
        pre_index(n, -1),
        post_index(n, -1),
        level() {
    tour.reserve(n);
    dfs(root, 0);
  }

 private:
  void dfs(int v, int lv) {
    pre_index[v] = int(tour.size());
    tour.push_back(v);
    level.push_back(lv);
    for (auto u : adj[v]) {
      dfs(u, lv + 1);
      tour.push_back(v);
      level.push_back(lv);
    }
    post_index[v] = int(tour.size());
  }
};

template <typename Monoid>
struct SegTree {
  using T = typename Monoid::T;

 private:
  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_

 public:
  inline int n() const { return n_; }
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
    for (int i = 0; i < st.n(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }
};

const int BIG = 1000000;

struct Min {
  using T = int;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return BIG; }
};

struct Max {
  using T = int;
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return -BIG; }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n, q;
  cin >> n >> q;
  vector<int> parent(n, -1);
  vector<vector<int>> children(n);
  REP(i, n - 1) {
    int p;
    cin >> p;
    --p;
    parent[i + 1] = p;
    children[p].push_back(i + 1);
  }
  REP(i, n) {
    DEBUG(i, parent[i]);
    DEBUG(i, children[i]);
  }
  EulerTour et(children, 0);
  DEBUG(et.tour);
  DEBUG(et.pre_index);
  DEBUG(et.post_index);
  DEBUG(et.level);

  SegTree<Min> minseg(et.pre_index);
  SegTree<Max> maxseg(et.pre_index);
  SegTree<Min> levelseg(et.level);

  REP(iq, q) {
    int l, r;
    cin >> l >> r;
    --l;
    DEBUG(l, r);
    int imin = minseg.fold(l, r);
    DEBUG(imin, et.tour[imin]);
    int imax = maxseg.fold(l, r);
    DEBUG(imax, et.tour[imax]);

    int manlv = -1;
    int excluded = -1;
    {  // exclude imin
      int ei = et.tour[imin];
      int mi = BIG;
      chmin(mi, minseg.fold(l, ei));
      chmin(mi, minseg.fold(ei + 1, r));
      assert(mi < BIG);
      DEBUG(imin, ei, mi);
      manlv = levelseg.fold(mi, imax + 1);
      excluded = ei;
    }
    {  // exclude imax
      int ei = et.tour[imax];
      int mi = -BIG;
      chmax(mi, maxseg.fold(l, ei));
      chmax(mi, maxseg.fold(ei + 1, r));
      assert(mi > -BIG);
      if (chmax(manlv, levelseg.fold(imin, mi + 1))) {
        excluded = ei;
      }
    }
    assert(excluded != -1);
    assert(manlv != -1);
    cout << excluded + 1 << " " << manlv << "\n";
  }
}
