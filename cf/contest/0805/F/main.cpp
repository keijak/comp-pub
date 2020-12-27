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
                        std::ostream *os = nullptr) {
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
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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
using Float = long double;

struct UnionFind {
  int n;
  mutable std::vector<int> par;  // positive: parent, negative: size
  int num_roots;

  explicit UnionFind(int sz) : n(sz), par(sz, -1), num_roots(sz) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (par[x] > par[y]) std::swap(x, y);
    par[x] += par[y];
    par[y] = x;
    --num_roots;
    return true;
  }

  int find(int k) const {
    if (par[k] < 0) return k;
    return par[k] = find(par[k]);
  }

  int size(int k) const { return -par[find(k)]; }

  bool same(int x, int y) const { return find(x) == find(y); }

  std::vector<int> roots() const {
    std::vector<int> res;
    res.reserve(num_roots);
    for (int i = 0; i < (int)par.size(); ++i) {
      if (par[i] < 0) res.push_back(i);
    }
    return res;
  }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);

  int n, m, q;
  cin >> n >> m >> q;
  vector<vector<int>> g(n);
  UnionFind uf(n);
  REP(i, m) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
    uf.unite(u, v);
  }

  vector<int> depth(n, 0), max_depth(n, 0);
  auto dfs2 = [&](auto self, int v, int p, int d) -> int {
    depth[v] = d;
    int md = d;
    for (auto w : g[v]) {
      if (w == p) continue;
      int dc = self(self, w, v, d + 1);
      chmax(md, dc);
    }
    max_depth[v] = md;
    return md;
  };

  vector<int> diameter_from(n, -1);
  auto dfs3 = [&](auto self, int v, int p, int pd) -> void {
    diameter_from[v] = max(max_depth[v] - depth[v], pd);
    int m = ssize(g[v]);
    vector<int> cuml(m + 1, 0), cumr(m + 1, 0);
    REP(i, m) {
      int w = g[v][i];
      int ri = m - 1 - i;
      if (w == p) {
        cuml[i + 1] = cuml[i];
        cumr[ri] = cumr[ri + 1];
      } else {
        cuml[i + 1] = max(cuml[i], max_depth[w] - depth[w] + 1);
        cumr[ri] = max(cumr[ri + 1], max_depth[w] - depth[w] + 1);
      }
    }
    REP(i, m) {
      int w = g[v][i];
      if (w == p) continue;
      int npd = 1 + max(pd, max(cuml[i], cumr[i + 1]));
      self(self, w, v, npd);
    }
  };

  vector<bool> done(n, false);
  REP(i, n) {
    int r = uf.find(i);
    if (done[r]) continue;
    done[r] = true;
    dfs2(dfs2, r, -1, 0);
    dfs3(dfs3, r, -1, 0);
  }
  DEBUG(depth);
  DEBUG(max_depth);
  DEBUG(diameter_from);

  map<int, vector<int>> path_length;
  vector<int> component_diameter(n, 0);
  REP(i, n) {
    int r = uf.find(i);
    int d = diameter_from[i];
    path_length[r].push_back(d);
    chmax(component_diameter[r], d);
  }

  map<u64, long double> cache;
  REP(qi, q) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    if (uf.same(u, v)) {
      cout << "-1\n";
      continue;
    }
    int ru = uf.find(u);
    int rv = uf.find(v);
    if (ru > rv) swap(ru, rv);
    u64 mkey = (u64(ru) << 20) | rv;
    auto it = cache.find(mkey);
    if (it != cache.end()) {
      cout << it->second << '\n';
      continue;
    }
    DEBUG(ru, component_diameter[ru], path_length[ru]);
    DEBUG(rv, component_diameter[rv], path_length[rv]);
    int base_d = max(component_diameter[ru], component_diameter[rv]);
    DEBUG(base_d);
    long double expected = 0.0;
    for (auto du : path_length[ru]) {
      for (auto dv : path_length[rv]) {
        long double dx = max(du + 1 + dv, base_d);
        expected += dx;
      }
    }
    expected /= ssize(path_length[ru]) * ssize(path_length[rv]);
    cout << expected << '\n';
    cache[mkey] = expected;
  }
}
