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

struct LowLink {
  using G = std::vector<std::vector<int>>;

  int n;
  G adj;
  std::vector<int> ord, low;
  std::vector<int> aps;  // articulation points
  std::vector<std::pair<int, int>> bridges;

  explicit LowLink(G g) : n(g.size()), adj(move(g)), ord(n, -1), low(n) {
    init();
  }

 private:
  void init() {
    int ord_counter = 0;

    auto dfs = [&](auto self, int v, int par = -1) -> void {
      ord[v] = ord_counter++;
      low[v] = ord[v];
      bool is_articulation_point = false;
      for (auto u : adj[v]) {
        if (ord[u] < 0) {
          self(self, u, v);
          low[v] = std::min(low[v], low[u]);
          if (par >= 0 and ord[v] <= low[u]) {
            is_articulation_point = true;
          }
          if (ord[v] < low[u]) {
            bridges.emplace_back(std::min(u, v), std::max(u, v));
          }
        } else if (u != par) {  // backward edge
          low[v] = std::min(low[v], ord[u]);
        }
      }
      // Is the root node an articulatin point?
      if (par < 0 and adj[v].size() >= 2) {
        is_articulation_point = true;
      }
      if (is_articulation_point) aps.push_back(v);
    };

    for (int i = 0; i < n; ++i) {
      if (ord[i] == -1) dfs(dfs, i);
    }

    // sort(aps.begin(), aps.end());
    // sort(bridges.begin(), bridges.end());
  }
};

using namespace std;

struct Graph {
  int n;
  V<V<int>> adj;
  map<pair<int, int>, int> edge_id;
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
      edge_id[{u, v}] = edge_id[{v, u}] = i;
    }
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  Graph g(N);
  g.input_undirected(N);
  LowLink lowlink(move(g.adj));

  set<int> non_bridges;
  REP(i, N) { non_bridges.insert(i); }
  for (auto [u, v] : lowlink.bridges) {
    int e = g.edge_id[{u, v}];
    non_bridges.erase(e);
  }
  cout << ssize(non_bridges) << '\n';
  bool first = true;
  for (auto x : non_bridges) {
    if (not first) cout << ' ';
    cout << x + 1;
    first = false;
  }
  cout << endl;
}
