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
template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
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
struct is_counterable : std::false_type {};
template <typename T>
struct is_counterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                     decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_counterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  V<int> par(N, -1);
  V<V<int>> child(N);
  REP(i, N - 1) {
    int p;
    cin >> p;
    --p;
    par[i + 1] = p;
    child[p].emplace_back(i + 1);
  }
  HLD hld(child);
  V<V<int>> subsizes(N);

  REP(i, N) {
    int v = hld.index_to_node[i];
    int cr = hld.comp_root[v];
    subsizes[cr].push_back(hld.subsize[v]);
  }
  DEBUG(subsizes);

  // centroid of subtree
  auto query = [&](int v) -> int {
    int total = hld.subsize[v];
    if (total <= 2) {
      return v;
    }
    int cr = hld.comp_root[v];
    int cr_index = hld.node_to_index[cr];
    int cid = hld.node_to_index[v] - cr_index;
    const auto &sz = subsizes[cr];
    int tv = cid, fv = ssize(sz);
    while (fv - tv > 1) {
      int mid = (fv + tv) / 2;
      if (sz[mid] > total / 2) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    int ans = hld.index_to_node[cr_index + tv];
    assert(hld.comp_root[ans] == cr);
    return ans;
  };

  REP(i, Q) {
    int v;
    cin >> v;
    --v;
    cout << query(v) + 1 << '\n';
  }
}
