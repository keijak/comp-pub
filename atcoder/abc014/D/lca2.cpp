#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
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
  return pprint(a, ", ", "", &(os << "{")) << "}";
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

// Lowest Common Ancestor by EulerTour + RMQ (SparseTable).
// - query: O(1)
// - build: O(N log N)
// - space: O(N log N)
struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  G adj;
  vector<int> depth;
  vector<int> index;
  // Euler Tour on nodes.
  vector<pair<int, int>> tour;  // (depth, node id)

  explicit EulerTour(G g, int root = 0)
      : n(g.size()), adj(move(g)), depth(n, 0), index(n, -1), tour() {
    tour.reserve(n * 2);
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    index[v] = int(tour.size());
    depth[v] = (p < 0) ? 0 : (depth[p] + 1);
    tour.emplace_back(depth[v], v);
    for (auto u : adj[v]) {
      if (u == p) continue;
      dfs(u, v);
      tour.emplace_back(depth[v], v);
    }
  }
};

template <class SemiLattice>
struct SparseTable {
  using T = typename SemiLattice::T;

  explicit SparseTable(const std::vector<T> &vec) { init(vec); }

  T fold(int l, int r) const {
    const T &lval = data_[height_[r - l]][l];
    const T &rval = data_[height_[r - l]][r - (1 << height_[r - l])];
    return SemiLattice::op(lval, rval);
  }

 private:
  void init(const std::vector<T> &vec) {
    int n = vec.size(), h = 0;
    while ((1 << h) < n) ++h;
    data_.assign(h, std::vector<T>(1 << h, SemiLattice::id()));
    height_.assign(n + 1, 0);
    for (int i = 2; i <= n; i++) {
      height_[i] = height_[i >> 1] + 1;
    }
    for (int i = 0; i < n; ++i) {
      data_[0][i] = vec[i];
    }
    for (int i = 1; i < h; ++i)
      for (int j = 0; j < n; ++j)
        data_[i][j] = SemiLattice::op(
            data_[i - 1][j], data_[i - 1][std::min(j + (1 << (i - 1)), n - 1)]);
  }

  std::vector<std::vector<T>> data_;
  std::vector<int> height_;
};

struct Min {
  using T = pair<int, int>;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() { return {std::numeric_limits<int>::max(), 0}; }
};

int euler_tour_lca_example(const EulerTour::G &g, int u, int v) {
  EulerTour et(g);
  SparseTable<Min> st(et.tour);

  // Usage
  int ui = et.index[u], vi = et.index[v];
  auto [lca_depth, lca_id] = st.fold(min(ui, vi), max(ui, vi) + 1);
  return lca_id;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    g[x].push_back(y);
    g[y].push_back(x);
  }
  EulerTour et(g);
  SparseTable<Min> st(et.tour);

  int Q;
  cin >> Q;
  REP(i, Q) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    int ai = et.index[a], bi = et.index[b];
    if (ai > bi) swap(ai, bi);
    auto [lca_depth, _] = st.fold(ai, bi);
    int cycle_len = (et.depth[a] - lca_depth) + (et.depth[b] - lca_depth) + 1;
    cout << cycle_len << '\n';
  }
}
