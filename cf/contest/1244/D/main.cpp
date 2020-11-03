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

const i64 INF = 1e18;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  auto cost = vector(3, vector(n, 0LL));
  REP(i, 3) {
    REP(j, n) { cin >> cost[i][j]; }
  }

  Graph g(n);
  g.input_undirected(n - 1);

  int start_node = -1, end_node = -1;
  REP(i, n) {
    int k = g.adj[i].size();
    if (k >= 3) {
      cout << -1 << endl;
      exit(0);
    }
    if (k == 1) {
      if (start_node == -1) {
        start_node = i;
      } else if (end_node == -1) {
        end_node = i;
      } else {
        cout << -1 << endl;
        exit(0);
      }
    }
  }

  V<int> next_node(n, -1), prev_node(n, -1);
  auto calc_next = [&](auto self, int v, int p) -> void {
    if (v == end_node) {
      return;
    }
    assert((int)g.adj[v].size() == ((v == start_node) ? 1 : 2));

    for (auto u : g.adj[v]) {
      if (u == p) continue;
      next_node[v] = u;
      prev_node[u] = v;
      self(self, u, v);
    }
  };
  calc_next(calc_next, start_node, -1);

  i64 ans_cost = INF;
  V<int> ans_colors;

  REP(i, 3) REP(j, 3) {
    if (i == j) continue;
    auto cumcost = vector(n, INF);
    auto colors = vector(n, -1);
    int v0 = start_node, v1 = next_node[v0];
    int c0 = i, c1 = j;
    cumcost[v0] = cost[c0][v0];
    cumcost[v1] = cumcost[v0] + cost[c1][v1];
    colors[v0] = c0;
    colors[v1] = c1;
    assert(v0 != end_node);
    assert(v1 != end_node);
    for (int v = next_node[v1];; v = next_node[v1]) {
      int c2 = 3 - c0 - c1;
      colors[v] = c2;
      cumcost[v] = cumcost[v1] + cost[c2][v];
      c0 = c1;
      c1 = c2;
      v0 = v1;
      v1 = v;
      if (v == end_node) break;
    }
    if (chmin(ans_cost, cumcost[end_node])) {
      ans_colors = colors;
    }
  }

  if (ans_cost >= INF) {
    cout << -1 << endl;
  } else {
    cout << ans_cost << '\n';
    REP(i, n) ans_colors[i]++;
    pprint(ans_colors);
  }
}
