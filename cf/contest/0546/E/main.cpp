#include <bits/stdc++.h>

#include <atcoder/maxflow>
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

const int INF = 1e5;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  V<int> a(n), b(n);
  cin >> a >> b;

  int source = 2 * n;
  int sink = 2 * n + 1;
  atcoder::mf_graph<int> g(2 * n + 2);
  REP(i, n) {
    g.add_edge(source, i, a[i]);
    g.add_edge(n + i, sink, b[i]);
    g.add_edge(i, n + i, a[i]);
  }

  REP(i, m) {
    int p, q;
    cin >> p >> q;
    p--;
    q--;
    g.add_edge(p, n + q, INF);
    g.add_edge(q, n + p, INF);
  }

  i64 asum = accumulate(ALL(a), 0LL);
  i64 bsum = accumulate(ALL(b), 0LL);
  if (asum != bsum) {
    cout << "NO" << endl;
    exit(0);
  }
  // DEBUG(asum);

  int maxflow = g.flow(source, sink);
  if (maxflow != bsum) {
    cout << "NO" << endl;
    exit(0);
  }
  // DEBUG(maxflow);

  auto ans = vector(n, vector(n, 0));
  auto edges = g.edges();
  for (const auto &e : edges) {
    if (e.to == sink) continue;
    if (e.from == source) continue;
    int i = e.from;
    int j = e.to - n;
    ans[i][j] = e.flow;
  }

  cout << "YES\n";
  REP(i, n) { pprint(ans[i]); }
}
