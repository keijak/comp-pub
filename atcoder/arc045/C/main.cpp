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

int N;
unsigned X;

struct Edge {
  int to;
  unsigned cost;
};

struct EulerTourOnEdge {
  using G = vector<vector<Edge>>;

  const int n;  // number of nodes
  G g;

  // Euler Tour on edges.
  vector<pair<unsigned, bool>> tour;  // (edge cost, entering?)
  vector<int> pre_index;   // index in the tour on entering the subtree of v
  vector<int> post_index;  // index in the tour on exiting the subtree of v
  vector<int> depth;

  explicit EulerTourOnEdge(G g, int root = 0)
      : n((int)g.size()),
        g(move(g)),
        pre_index(n, -1),
        post_index(n, -1),
        depth(n, -1) {
    tour.reserve(n * 2);
    depth[root] = 0;
    dfs(root, -1);
  }

 private:
  void dfs(int v, int p) {
    pre_index[v] = int(tour.size());
    if (p >= 0) depth[v] = depth[p] + 1;
    for (const Edge &e : g[v]) {
      if (e.to == p) continue;
      tour.push_back({e.cost, true});
      dfs(e.to, v);
      tour.push_back({e.cost, false});
    }
    post_index[v] = int(tour.size());
  }
};

i64 solve() {
  cin >> N >> X;
  vector<vector<Edge>> g(N);
  REP(i, N - 1) {
    int x, y;
    unsigned c;
    cin >> x >> y >> c;
    --x, --y;
    g[x].push_back({y, c});
    g[y].push_back({x, c});
  }
  EulerTourOnEdge et(g);
  map<unsigned, int> counts;
  ++counts[0];
  unsigned acc = 0;
  i64 ans = 0;
  for (const auto &[co, into_node] : et.tour) {
    acc ^= co;
    if (into_node) {
      if (auto it = counts.find(acc ^ X); it != counts.end()) {
        ans += it->second;
      }
      ++counts[acc];
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
