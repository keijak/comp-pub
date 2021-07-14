#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

#ifdef MY_DEBUG
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
// The interval [begin[v], end[v]) represents a subtree whose
// root is v.
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G g;

  // Euler Tour on nodes.
  vector<pair<int, int>> tour;  // (depth, node id)
  vector<int> begin;  // index in the tour on entering the subtree of v
  vector<int> end;    // index in the tour on exiting the subtree of v
  vector<int> depth;

  explicit EulerTour(G g, int root = 0)
      : n((int)g.size()), g(move(g)), begin(n, -1), end(n, -1), depth(n, -1) {
    tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    begin[v] = int(tour.size());
    if (p >= 0) depth[v] = depth[p] + 1;
    tour.emplace_back(depth[v], v);
    for (auto u : g[v]) {
      if (u == p) continue;
      dfs(u, v);
      // tour.emplace_back(depth[v], v);
    }
    end[v] = int(tour.size());
  }
};

template <typename Monoid>
struct DualSegTree {
  using T = typename Monoid::T;

  explicit DualSegTree(int n) : size_(n), data_(2 * n, Monoid::id()) {}

  inline int size() const { return size_; }

  // Returns i-th value.
  T operator[](int i) const {
    int k = size_ + i;
    T res = data_[k];
    while (k > 1) {
      k >>= 1;
      res = Monoid::op(res, data_[k]);
    }
    return res;
  }

  // Applies a value to [l,r) range.
  void apply(int l, int r, T val) {
    l += size_;
    r += size_;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) {
        data_[l] = Monoid::op(data_[l], val);
        ++l;
      }
      if (r & 1) {
        --r;
        data_[r] = Monoid::op(data_[r], val);
      }
    }
  }

 private:
  int size_;
  std::vector<T> data_;
};
template <typename T>
std::ostream &operator<<(std::ostream &os, const DualSegTree<T> &st) {
  os << "[";
  for (int i = 0; i < st.size(); ++i) {
    if (i != 0) os << ", ";
    os << st[i];
  }
  return os << "]";
}

struct Sum {
  using T = long long;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vector<int> a(n);
  cin >> a;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int v, u;
    cin >> v >> u;
    --v, --u;
    g[v].push_back(u);
    g[u].push_back(v);
  }

  EulerTour et(g);
  assert(ssize(et.tour) == n);

  DualSegTree<Sum> seg(n);
  REP(v, n) {
    int i = et.begin[v];
    int val = a[v];
    if (et.depth[v] & 1) {
      val *= -1;
    }
    seg.apply(i, i + 1, val);
  }

  REP(qi, m) {
    int qt, x, val;
    cin >> qt >> x;
    --x;
    const int xd = et.depth[x];
    if (qt == 1) {
      cin >> val;
      if (xd & 1) {
        val *= -1;
      }
      seg.apply(et.begin[x], et.end[x], val);
    } else {
      int j = et.begin[x];
      i64 val = seg[j];
      if (xd & 1) {
        val *= -1;
      }
      cout << val << "\n";
    }
  }
}
