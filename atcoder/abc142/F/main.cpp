#include <bits/stdc++.h>

#include <atcoder/scc>
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  V<V<int>> adj(n);
  atcoder::scc_graph g(n);
  REP(i, m) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;
    g.add_edge(u, v);
    adj[u].push_back(v);
  }

  auto components = g.scc();

  const int INF = 1e9;
  int min_cycle = INF;
  V<int> ans;

  REP(i, components.size()) {
    const auto &c = components[i];
    if (c.size() <= 1) continue;
    // DFS
    const set<int> cset(ALL(c));
    set<int> visited;
    V<int> path;

    auto dfs = [&](auto self, int cur, int index, int root) -> void {
      for (auto v : adj[cur]) {
        if (v == cur) continue;
        if (v == root) {
          if (chmin(min_cycle, index)) ans = path;
          continue;
        }
        if (not cset.count(v)) continue;
        bool inserted = visited.insert(v).second;
        if (not inserted) continue;
        if (index + 1 < min_cycle) {
          path.push_back(v);
          self(self, v, index + 1, root);
          path.pop_back();
        }
      }
    };
    for (auto r : c) {
      visited.clear();
      path.clear();
      visited.insert(r);
      path.push_back(r);
      dfs(dfs, r, 0, r);
    }
  }

  if (min_cycle == INF) {
    cout << -1 << endl;
  } else {
    cout << ans.size() << '\n';
    for (auto z : ans) cout << z + 1 << '\n';
  }
}
