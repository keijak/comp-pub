#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

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

#ifdef MY_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &... args) {
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
  explicit Graph(int n) : n(n), adj(n) {}
  void input_undirected(int m) { input(m, false); }
  void input_directed(int m) { input(m, true); }

 private:
  void input(int m, bool is_directed) {
    for (int i = 0; i < m; ++i) {
      int u, v;  // read as 1-indexed.
      cin >> u >> v;
      u--, v--;  // to 0-indexed.
      adj[u].push_back(v);
      if (not is_directed) adj[v].push_back(u);
    }
  }
};

struct UnionFind {
  vector<int> data;  // positive: parent, negative: size
  int num_roots;

  explicit UnionFind(size_t sz) : data(sz, -1), num_roots(sz) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (data[x] > data[y]) swap(x, y);
    data[x] += data[y];
    data[y] = x;
    --num_roots;
    return true;
  }

  int find(int k) {
    if (data[k] < 0) return k;
    return data[k] = find(data[k]);
  }

  int size(int k) { return -data[find(k)]; }

  bool same(int x, int y) { return find(x) == find(y); }

  vector<int> roots() {
    vector<int> res;
    res.reserve(num_roots);
    for (int i = 0; i < (int)data.size(); ++i) {
      if (data[i] < 0) res.push_back(i);
    }
    return res;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  Graph g(n);
  g.input_undirected(n - 1);

  auto calc_path = [&](auto self, int v, int pv, V<int> &path) -> bool {
    path.push_back(v);
    if (v == n - 1) {
      return true;
    }
    for (auto u : g.adj[v]) {
      if (u == pv) continue;
      auto found = self(self, u, v, path);
      if (found) return true;
    }
    assert(path.back() == v);
    path.pop_back();
    return false;
  };

  V<int> path;
  auto path_ok = calc_path(calc_path, 0, -1, path);
  assert(path_ok);
  DEBUG(path);
  int i, j;
  for (i = 0, j = SIZE(path) - 1; i + 1 < j;) {
    ++i;
    if (i < j - 1) --j;
  }
  int cut1 = path[i], cut2 = path[j];

  UnionFind uf(n);
  REP(k, n) {
    for (auto u : g.adj[k]) {
      if ((k == cut1 and u == cut2) or (k == cut2 and u == cut1)) continue;
      if (k < u) {
        uf.unite(k, u);
      }
    }
  }
  assert(not uf.same(0, n - 1));
  if (uf.size(0) > uf.size(n - 1)) {
    cout << "Fennec\n";
  } else {
    cout << "Snuke\n";
  }
}
