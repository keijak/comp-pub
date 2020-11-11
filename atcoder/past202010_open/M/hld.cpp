#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

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

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
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

struct Graph {
  int n;
  V<V<int>> adj;
  std::map<pair<int, int>, int> edge_id;

  explicit Graph(int n) : n(n), adj(n), edge_id() {}

  void input_undirected(int m) {
    for (int i = 0; i < m; ++i) {
      int u, v;  // read as 1-indexed.
      cin >> u >> v;
      u--, v--;  // to 0-indexed.
      adj[u].push_back(v);
      adj[v].push_back(u);
      edge_id[{u, v}] = edge_id[{v, u}] = i;
    }
  }
};

// Heavy-Light Decomposition and Euler Tour
struct HLD {
  int n;
  std::vector<std::vector<int>> adj;
  std::vector<int> subsize, parent, node_to_index, index_to_node, comp_root;
  int root;
  int counter;

  explicit HLD(std::vector<std::vector<int>> g, int root = 0)
      : n(int(g.size())),
        adj(g),
        subsize(n, 1),
        parent(n, -1),
        node_to_index(n, -1),
        index_to_node(n, -1),
        comp_root(n, -1),
        root(root) {
    dfs_subsize(root);
    int counter = 0;
    comp_root[root] = root;
    dfs_hld(root, counter);
  }

  int lca(int u, int v) {
    for (;;) {
      if (node_to_index[u] > node_to_index[v]) std::swap(u, v);
      if (comp_root[u] == comp_root[v]) return u;
      v = parent[comp_root[v]];
    }
  }

  // path [u, v]
  template <typename F>
  void for_each(int u, int v, const F &f) {
    for (;;) {
      if (node_to_index[u] > node_to_index[v]) std::swap(u, v);
      f(std::max(node_to_index[comp_root[v]], node_to_index[u]),
        node_to_index[v] + 1);
      if (comp_root[u] == comp_root[v]) break;
      v = parent[comp_root[v]];
    }
  }

  template <typename F>
  void for_each_edge(int u, int v, const F &f) {
    for (;;) {
      if (node_to_index[u] > node_to_index[v]) std::swap(u, v);
      if (comp_root[u] == comp_root[v]) {
        if (u != v) f(node_to_index[u] + 1, node_to_index[v] + 1);
        break;
      }
      f(node_to_index[comp_root[v]], node_to_index[v] + 1);
      v = parent[comp_root[v]];
    }
  }

 private:
  // Fills `subsize` and `parent`.
  void dfs_subsize(int v) {
    auto &edges = adj[v];
    if (parent[v] >= 0) {
      // Drop the parent from `adj`. It's separately stored in `parent`.
      auto it = std::find(edges.begin(), edges.end(), parent[v]);
      if (it != edges.end()) edges.erase(it);
    }
    for (int &u : edges) {
      parent[u] = v;
      dfs_subsize(u);
      subsize[v] += subsize[u];
      if (subsize[u] > subsize[edges[0]]) {
        std::swap(u, edges[0]);
      }
    }
  }

  // Fills `node_to_index`, `index_to_node`, and `comp_root`.
  void dfs_hld(int v, int &counter) {
    node_to_index[v] = counter++;
    index_to_node[node_to_index[v]] = v;
    for (int u : adj[v]) {
      assert(u != parent[v]);
      comp_root[u] = (u == adj[v][0] ? comp_root[v] : u);
      dfs_hld(u, counter);
    }
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

struct Max {
  using T = pair<int, int>;  // (time, color)
  static T op(const T &x, const T &y) { return std::max(x, y); }
  static constexpr T id() { return {-1, 0}; }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  Graph g(N);
  g.input_undirected(N - 1);
  HLD hld(g.adj);
  DualSegTree<Max> seg(N);
  REP(i, Q) {
    int u, v, c;
    cin >> u >> v >> c;
    --u;
    --v;
    hld.for_each_edge(u, v, [&](int l, int r) { seg.apply(l, r, {i, c}); });
  }
  V<int> ans(N - 1);
  REP(v, N) {
    if (hld.parent[v] < 0) continue;
    int e = g.edge_id[{hld.parent[v], v}];
    int i = hld.node_to_index[v];
    ans[e] = seg[i].second;
  }
  pprint(ans, "\n");
}
